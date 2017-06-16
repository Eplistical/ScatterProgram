#ifndef _MATRIXOP_HPP
#define _MATRIXOP_HPP
/* module for matrix opeations
 * require C++11
 * written based on vector.hpp
 * using external lib: 
 * 	mkl
 *
 * Gaohan
 */

#define MKL_Complex16 std::complex<double>
#include <cmath>
#include <complex>
#include <mkl.h>
#include "vector.hpp"

namespace matrixop{
	using namespace std;

	inline void hdiag(const vector<double>& MatrixToDiag, vector<double>& EigenVal, vector<double>& EigenVec){
		/* diag a real-symmetric matrix
		 *
		 * 	param MatrixToDiag: matrix to be diag, column-major, must have N*N elements
		 * 	param EignVal, EigenVac: vector to store results
		 * */ 
		vector<double> A(MatrixToDiag);
		const int N = static_cast<size_t>(sqrt(A.size()));
		// helpers
		const char UPLO = 'U', COMPZ = 'V';
		vector<double> d(N), e(N), tau(N), work(N);
		vector<int> iwork(N);
		int lwork, liwork, info;
		// get tri-diagonal, A = QTQ'
		lwork = -1;
		dsytrd(&UPLO, &N, &A[0], &N, &d[0], &e[0], &tau[0], &work[0], &lwork, &info);
		lwork = static_cast<int>(work[0]);
		work.resize(lwork);
		dsytrd(&UPLO, &N, &A[0], &N, &d[0], &e[0], &tau[0], &work[0], &lwork, &info);
		// generate unitary matrix Q
		lwork = -1;
		dorgtr(&UPLO, &N, &A[0], &N, &tau[0], &work[0], &lwork, &info);
		lwork = static_cast<int>(work[0]);
		work.resize(lwork);
		dorgtr(&UPLO, &N, &A[0], &N, &tau[0], &work[0], &lwork, &info);
		// find eva & A
		lwork = -1;
		liwork = -1;
		dstedc(&COMPZ, &N, &d[0], &e[0], &A[0], &N, &work[0], &lwork, &iwork[0], &liwork, &info);
		lwork = static_cast<int>(work[0]);
		work.resize(lwork);
		liwork = iwork[0];
		iwork.resize(liwork);
		dstedc(&COMPZ, &N, &d[0], &e[0], &A[0], &N, &work[0], &lwork, &iwork[0], &liwork, &info);
		// eva = d
		EigenVal.swap(d);
		EigenVec.swap(A);
	}

	inline vector<double> dmv(const vector<double>& m, const vector<double>& v, char TRANS = 'N') {
		/*  matrix-vector multiplication
		 *
		 * 	param m: matrix of M * N shape
		 * 	param v: vector of N length
		 * */ 
		const int N = v.size();
		const int M = static_cast<int>(m.size() / N);
		vector<double> rst(M);
		const double alpha = 1.0, beta = 0.0;
		const int incx = 1;
		dgemv(&TRANS, &M, &N, &alpha, &m[0], &N, 
					&v[0], &incx, &beta, &rst[0], &incx);
		return rst;
    }
};

#endif // _MATRIXOP_HPP
