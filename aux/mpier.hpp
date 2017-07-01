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
#include <typeindex>
#include <unordered_map>
#include <type_traits>
#include <stdint.h>
#include <limits.h>
#include <boost/type_traits.hpp> 
#include <boost/utility/enable_if.hpp>
#include "mpi.h"


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

	// normal type -> MPI TYPE mapping
	using MPI_PREDIFINED_DATA_T = decltype(MPI_INT);
	std::unordered_map<type_index, MPI_PREDIFINED_DATA_T> typemap
	= { 
		{typeid(char), MPI_CHAR},
		{typeid(int), MPI_INT},
		{typeid(long), MPI_LONG},
		{typeid(long long), MPI_LONG_LONG},
		{typeid(size_t), MPI_SIZE_T},
		{typeid(float), MPI_FLOAT},
		{typeid(double), MPI_DOUBLE},
	  };

	int size;
	int rank;
	int master;

	inline void setup(void) {
		MPI::Init();
		size = MPI::COMM_WORLD.Get_size();
		rank = MPI::COMM_WORLD.Get_rank();
		master = (rank == 0);
	}

	inline void barrier(void) {
		MPI::COMM_WORLD.Barrier();
	}

	inline void finalize(void) {
		MPI::Finalize();
	}

	
	inline void bcast(int& root){  }

	template<typename ParamType>
		inline void bcast(int root, ParamType& x){
			MPI::COMM_WORLD.Bcast(&x, 1, typemap[typeid(ParamType)], root);
		}

	template<typename ParamType, typename ... Types>
		inline void bcast(int root, ParamType& x, Types& ... otherx){
			bcast(root, x);
			bcast(root, otherx ...);
		}
};


#endif
