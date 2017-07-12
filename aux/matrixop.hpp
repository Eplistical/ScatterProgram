#ifndef _MATRIXOP_HPP
#define _MATRIXOP_HPP
/* module for matrix opeations
 * require C++14
 * written based on vector.hpp
 * using external lib: 
 * 	Eigen
 *
 * Gaohan
 */

#include <cmath>
#include <cassert>
#include "vector.hpp"
#include "Eigen/Eigenvalues"
#include "Eigen/Core"

namespace matrixop{
	using namespace Eigen;
	using namespace std;

	template<typename MatrixElementType, typename EigenValType, typename EigenVecType>
		inline void hdiag(const vector<MatrixElementType>& MatrixToDiag, 
								vector<EigenValType>& EigenVal, 
								vector<EigenVecType>& EigenVec)
		{
			/* diag a symmetric matrix
			 *
			 * 	param MatrixToDiag: matrix to be diag, column-major
			 * 	param EignVal, EigenVac: vector to store results
			 * */ 
			using MatrixType = Matrix<MatrixElementType, Dynamic, Dynamic>;
			const int N = static_cast<uint_fast32_t>(sqrt(MatrixToDiag.size()));
			Map<const MatrixType> A(MatrixToDiag.data(), N, N);
			SelfAdjointEigenSolver<MatrixType> es(A);
			EigenVec = vector<EigenVecType>(es.eigenvectors().data(), 
					es.eigenvectors().data() + es.eigenvectors().size());
			EigenVal = vector<EigenValType>(es.eigenvalues().data(), 
					es.eigenvalues().data() + es.eigenvalues().size());
		}

	template<typename MatrixElementType>
		inline vector<MatrixElementType> _matmat(
				 const vector<MatrixElementType>& Mat1, 
				 const vector<MatrixElementType>& Mat2,
				 const uint_fast32_t M, const uint_fast32_t N, const uint_fast32_t K)
		{
			/*  matrix-matrix multiplication, 
			 *  should not be called by external functions
			 *
			 * 	param Mat1: matrix of M * N shape
			 * 	param Mat2: matrix of N * K shape
			 * 	param M, N, K: dimensions of matrices
			 *
			 * 	return a std::vector storing resulting M * K matrix
			 * */ 

			// check size
			assert(Mat1.size() >= M * N);
			assert(Mat2.size() >= N * K);
			// determine return type
			using MatrixType = Matrix<MatrixElementType, Dynamic, Dynamic>;
			Map<const MatrixType> m1(Mat1.data(), M, N);
			Map<const MatrixType> m2(Mat2.data(), N, K);
			MatrixType rst = m1 * m2;
			return vector<MatrixElementType>(rst.data(), rst.data() + rst.size());
		}

	template<typename MatrixElementType>
		inline vector<MatrixElementType> matmat( 
				const vector<MatrixElementType>& Mat1, 
				const vector<MatrixElementType>& Mat2,
				const uint_fast32_t N)
		{
			/*  matrix-matrix multiplication, 
			 *
			 * 	param Mat1: matrix with N cols
			 * 	param Mat2: matrix with N rows
			 * 	param N:	col number for Mat1 
			 *
			 * 	return a std::vector storing resulting M * K matrix
			 * */ 
			const uint_fast32_t M = static_cast<const uint_fast32_t>(Mat1.size() / N);
			const uint_fast32_t K = static_cast<const uint_fast32_t>(Mat2.size() / N);
			return _matmat(Mat1, Mat2, M, N, K);
		}

	template<typename MatrixElementType>
		inline vector<MatrixElementType> matvec( 
				const vector<MatrixElementType>& Mat, 
				const vector<MatrixElementType>& Vec)
		{
			/*  matrix-matrix multiplication, 
			 *
			 * 	param Mat: matrix with M * N cols
			 * 	param Vec: vector with N elements
			 *
			 * 	return a std::vector with M elements
			 * */ 
			const uint_fast32_t N = static_cast<const uint_fast32_t>(Vec.size());
			const uint_fast32_t M = static_cast<const uint_fast32_t>(Mat.size() / N);
			return _matmat(Mat, Vec, M, N, 1);
		}
};

#endif // _MATRIXOP_HPP
