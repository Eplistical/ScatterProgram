#ifndef _MPIER_HPP
#define _MPIER_HPP
/* module for MPIer
 *
 * used boost/type traits
 *
 * Gaohan
 */
#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <stdint.h>
#include <limits.h>
#include "mpi.h"
#include "type_traiter.hpp"


#if SIZE_MAX == UCHAR_MAX
#define MPI_SIZE_T MPI_UNSIGNED_CHAR
#elif SIZE_MAX == USHRT_MAX
#define MPI_SIZE_T MPI_UNSIGNED_SHORT
#elif SIZE_MAX == UINT_MAX
#define MPI_SIZE_T MPI_UNSIGNED
#elif SIZE_MAX == ULONG_MAX
#define MPI_SIZE_T MPI_UNSIGNED_LONG
#elif SIZE_MAX == ULLONG_MAX
#define MPI_SIZE_T MPI_UNSIGNED_LONG_LONG
#else
#error "Cannot determine MPI_SIZE_T"
#endif

namespace mpier{
	using namespace std;
	using namespace type_traiter;

	// -- vars -- //
	// normal type -> MPI TYPE mapping
	using MPI_PREDIFINED_DATA_T = decltype(MPI_INT);
	static std::unordered_map<type_index, MPI_PREDIFINED_DATA_T> typemap
		= { 
			{typeid(char), MPI_CHAR},
			{typeid(int), MPI_INT},
			{typeid(long), MPI_LONG},
			{typeid(long long), MPI_LONG_LONG},
			{typeid(size_t), MPI_SIZE_T},
			{typeid(float), MPI_FLOAT},
			{typeid(double), MPI_DOUBLE},
		};

	static int size;
	static int rank;
	static bool master;

	// -- helpers -- //
	inline std::vector<size_t> assign_job(size_t Njob)
	{
		/**
		 * given a total Njobs
		 * return a vector {StartJobIndex, JobNumber} for current process
		 */
		size_t quotient = Njob / size;
		size_t remainder = Njob % size;
		std::vector<size_t> rst(2);
		if (rank < remainder) {
			rst[0] = (quotient + 1) * rank;
			rst[1] = quotient + 1;
		}
		else {
			rst[0] = quotient * rank + remainder;
			rst[1] = quotient;
		}
		return rst;
	}

	template<typename T>
	inline std::vector<T> assign_job(std::vector<T> Jobs)
	{
		/**
		 * given a vector of all jobs,
		 * return a vector of jobs for current process
		 */
		std::vector<size_t> mybatch = assign_job(Jobs.size());
		return std::vector<T>(Jobs.begin() + mybatch[0], Jobs.begin() + mybatch[0] + mybatch[1]);
	}

	// -- init/finalize --//
	inline void setup(void) 
	{
		MPI::Init();
		size = MPI::COMM_WORLD.Get_size();
		rank = MPI::COMM_WORLD.Get_rank();
		master = (rank == 0);
	}

	inline void finalize(void) 
	{
		MPI::Finalize();
	}

	// -- barrier -- //
	inline void barrier(void) 
	{
		MPI::COMM_WORLD.Barrier();
	}


	// -- bcast -- //
	inline void bcast(int& root){  }

	template<typename ParamType>
		inline typename enable_if<is_fundamental<ParamType>::value && (!is_bool<ParamType>::value), void>::type
		bcast(int root, ParamType& x)
		{
			MPI::COMM_WORLD.Bcast(&x, 1, typemap[typeid(ParamType)], root);
		}

	template<typename ParamType>
		inline typename enable_if<is_bool<ParamType>::value, void>::type
		bcast(int root, ParamType& x) 
		{
			int tmp = static_cast<int>(x);
			MPI::COMM_WORLD.Bcast(&tmp, 1, MPI_INT, root);
			x = static_cast<bool>(tmp);
		}

	template<typename ParamType>
		inline typename enable_if<is_vector<ParamType>::value || is_string<ParamType>::value, void>::type
		bcast(int root, ParamType& x)
		{
			size_t _size = x.size();
			MPI::COMM_WORLD.Bcast(&_size, 1, MPI_SIZE_T, root);
			if (not master) x.resize(_size);
			MPI::COMM_WORLD.Bcast(&x[0], x.size(), typemap[typeid(typename ParamType::value_type)], root);
		}

	template<typename ParamType, typename ... Types>
		inline void bcast(int root, ParamType& x, Types& ... otherx)
		{
			bcast(root, x);
			bcast(root, otherx ...);
		}
};


#endif
