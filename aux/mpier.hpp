#ifndef _MPIER_HPP
#define _MPIER_HPP
/* module for MPIer
 *
 * used boost/type traits
 *
 * Gaohan
 */
#include "types.hpp"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <stdint.h>
#include <limits.h>
#include "mpi.h"
#include "type_traiter.hpp"

namespace mpier{
	using namespace std;
	using namespace type_traiter;

	// -- vars -- //
	// normal type -> MPI TYPE mapping
	using MPI_PREDIFINED_DATA_T = decltype(MPI_INT);
	static std::unordered_map<type_index, MPI_PREDIFINED_DATA_T> typemap
		= { 
			{typeid(CHAR_T), MPI_CHAR},
			{typeid(INT_T), MPI_INT32_T},
			{typeid(UINT_T), MPI_UINT32_T},
			{typeid(DOUBLE_T), MPI_DOUBLE},
		};

	static INT_T size;
	static INT_T rank;
	static bool master;

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
	inline void bcast(INT_T root){  }

	template<typename ParamType>
		inline typename enable_if<is_fundamental<ParamType>::value && (!is_bool<ParamType>::value), void>::type
		bcast(INT_T root, ParamType& x)
		{
			MPI::COMM_WORLD.Bcast(&x, 1, typemap[typeid(ParamType)], root);
		}

	template<typename ParamType>
		inline typename enable_if<is_bool<ParamType>::value, void>::type
		bcast(INT_T root, ParamType& x) 
		{
			INT_T tmp = static_cast<INT_T>(x);
			MPI::COMM_WORLD.Bcast(&tmp, 1, MPI_UINT32_T, root);
			x = static_cast<bool>(tmp);
		}

	template<typename ParamType>
		inline typename enable_if<is_vector<ParamType>::value || is_string<ParamType>::value, void>::type
		bcast(INT_T root, ParamType& x)
		{
			UINT_T _size = x.size();
			MPI::COMM_WORLD.Bcast(&_size, 1, MPI_UINT32_T, root);
			if (not master) x.resize(_size);
			MPI::COMM_WORLD.Bcast(&x[0], x.size(), typemap[typeid(typename ParamType::value_type)], root);
		}

	template<typename ParamType, typename ... Types>
		inline void bcast(INT_T root, ParamType& x, Types& ... otherx)
		{
			bcast(root, x);
			bcast(root, otherx ...);
		}


	// -- utilities -- //
	inline std::vector<UINT_T> assign_job_start_and_num(UINT_T Njob)
	{
		/**
		 * given a total Njobs
		 * return a vector {StartJobIndex, JobNumber} for current process
		 */
		UINT_T quotient = Njob / size;
		UINT_T remainder = Njob % size;
		std::vector<UINT_T> rst(2);
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
		std::vector<UINT_T> mybatch = assign_job_start_and_num(Jobs.size());
		return std::vector<T>(Jobs.begin() + mybatch[0], Jobs.begin() + mybatch[0] + mybatch[1]);
	}

	inline std::vector<UINT_T> assign_job(UINT_T Njob)
	{
		/**
		 * given total # of jobs
		 * return a vector continuous job indices
		 */
		vector<UINT_T> Jobs(Njob);
		if (master) {
			for (UINT_T i = 0; i < Jobs.size(); ++i) 
				Jobs[i] = i;
		}
		bcast(0, Jobs);
		std::vector<UINT_T> mybatch = assign_job(Jobs);
		return mybatch;
	}

	inline std::vector<UINT_T> assign_job_random(UINT_T Njob)
	{
		/**
		 * given total # of jobs
		 * return a vector of random job indices
		 */
		vector<UINT_T> Jobs(Njob);
		if (master) {
			for (UINT_T i = 0; i < Jobs.size(); ++i) 
				Jobs[i] = i;
			std::random_shuffle(Jobs.begin(), Jobs.end()); 
		}
		bcast(0, Jobs);
		std::vector<UINT_T> mybatch = assign_job(Jobs);
		return mybatch;
	}

};


#endif
