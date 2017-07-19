#ifndef _MY_VECTOR_HPP
#define _MY_VECTOR_HPP
// for vector
#include <cmath>
#include <vector>
#include <stdexcept>

template<typename T1, typename T2>
void assertsize(const std::vector<T1>& v1, const std::vector<T2>& v2)
{
    if(v1.size() != v2.size())
        throw std::runtime_error("assertsize: v1 & v2 have different size!");
}

template<typename T1, typename T2>
auto operator+(const std::vector<T1>& v1, const std::vector<T2>& v2) -> std::vector<decltype(v1.at(0) + v2.at(0))>
{
    const size_t N = v1.size();
    assertsize(v1, v2);
    std::vector<decltype(v1.at(0) + v2.at(0))> rst(N);
    for(size_t j = 0; j < N; j++)
        rst[j] = v1[j] + v2[j];
	return rst;
}

template<typename T1, typename T2>
std::vector<T1> operator+(const std::vector<T1>& v1, const T2& a)
{
    const size_t N = v1.size();
    std::vector<T1> rst(N);
    for(size_t j = 0; j < N; j++)
        rst[j] = v1[j] + a;
	return rst;
}

template<typename T1, typename T2>
std::vector<T1> operator+(const T2& a, const std::vector<T1>& v1)
{
	return v1 + a;
}

template<typename T1, typename T2>
auto operator-(const std::vector<T1>& v1, const std::vector<T2>& v2) -> std::vector<decltype(v1.at(0) - v2.at(0))>
{
    const size_t N = v1.size();
    assertsize(v1, v2);
    std::vector<decltype(v1.at(0) - v2.at(0))> rst(N);
    for(size_t j = 0; j < N; j++)
        rst[j] = v1[j] - v2[j];
	return rst;
}

template<typename T1, typename T2>
std::vector<T1> operator-(const std::vector<T1>& v1, const T2& a)
{
    const size_t N = v1.size();
    std::vector<T1> rst(N);
    for(size_t j = 0; j < N; j++)
        rst[j] = v1[j] - a;
	return rst;
}

template<typename T1, typename T2>
std::vector<T1> operator-(const T2& a, const std::vector<T1>& v1){
    const size_t N = v1.size();
    std::vector<T1> rst(N);
    for(size_t j = 0; j < N; j++)
        rst[j] = a - v1[j];
	return rst;
}

template<typename T>
std::vector<T> operator-(const std::vector<T>& v1)
{
    const size_t N = v1.size();
    std::vector<T> rst(N);
    for(size_t j = 0; j < N; j++)
        rst[j] = -v1[j];
	return rst;
}

template<typename T1, typename T2>
auto operator*(const std::vector<T1>& v1, const std::vector<T2>& v2) -> std::vector<decltype(v1.at(0) * v2.at(0))>
{
    const size_t N = v1.size();
    assertsize(v1, v2);
    std::vector<decltype(v1.at(0) * v2.at(0))> rst(N);
    for(size_t j = 0; j < N; j++)
        rst[j] = v1[j] * v2[j];
	return rst;
}

template<typename T1, typename T2>
std::vector<T1> operator*(const std::vector<T1>& v1, const T2& a)
{
    const size_t N = v1.size();
    std::vector<T1> rst(N);
    for(size_t j = 0; j < N; j++)
        rst[j] = v1[j] * a;
	return rst;
}

template<typename T1, typename T2>
std::vector<T1> operator*(const T2& a, const std::vector<T1>& v1)
{
	return v1 * a;
}

template<typename T1, typename T2>
auto operator/(const std::vector<T1>& v1, const std::vector<T2>& v2) -> std::vector<decltype(v1.at(0) / v2.at(0))>
{
    const size_t N = v1.size();
    assertsize(v1, v2);
    std::vector<decltype(v1.at(0) / v2.at(0))> rst(N);
    for(size_t j = 0; j < N; j++)
        rst[j] = v1[j] / v2[j];
	return rst;
}

template<typename T1, typename T2>
std::vector<T1> operator/(const std::vector<T1>& v1, const T2& a)
{
    const size_t N = v1.size();
    std::vector<T1> rst(N);
    for(size_t j = 0; j < N; j++)
        rst[j] = v1[j] / a;
	return rst;
}

template<typename T1, typename T2>
std::vector<T1> operator/(const T2& a, const std::vector<T1>& v1)
{
    const size_t N = v1.size();
    std::vector<T1> rst(N);
    for(size_t j = 0; j < N; j++)
        rst[j] = a / v1[j];
	return rst;
}

template<typename T>
std::vector<T> abs(const std::vector<T>& v)
{
    const size_t N = v.size();
    std::vector<T> rst(N);
    for(size_t j = 0;j < N;j++) 
		rst[j] = abs(v[j]) ;
    return rst;
}

template<typename T1, typename T2>
std::vector<T1> pow(const std::vector<T1>& v1, const T2& a)
{
    const size_t N = v1.size();
    std::vector<T1> rst(N);
    for(size_t j = 0; j < N; j++)
        rst[j] = pow(v1[j], a);
	return rst;
}

template<typename T>
std::vector<T> subvec(const std::vector<T>& v, size_t start, size_t N)
{
    return std::vector<T>(&v.at(start), &v.at(start + N));
}

template<typename T>
T sum(const std::vector<T>& v)
{
    T rst = static_cast<T>(0);
    const size_t N = v.size();
    for(size_t j = 0;j < N;j++) 
		rst += v.at(j);
    return rst;
}

template<typename T>
T mean(const std::vector<T>& v)
{
    return sum(v) / v.size();
}

template<typename T>
T product(const std::vector<T>& v)
{
    T rst = static_cast<T>(1);
    const size_t N = v.size();
    for(size_t j = 0;j < N;j++) 
		rst *= v.at(j);
    return rst;
}

template<typename T>
T norm(const std::vector<T>& v)
{
    return sqrt(sum(v * v));
}

template<typename T>
T min(const std::vector<T>& v)
{
    const size_t N = v.size();
    T rst = v.at(0);
    for(int j = 1;j < N;j++)
        rst = (v[j] < rst)?v[j]:rst;
    return rst;
}

template<typename T>
T max(const std::vector<T>& v)
{
    const size_t N = v.size();
    T rst = v.at(0);
    for(int j = 1;j < N;j++)
        rst = (v[j] > rst)?v[j]:rst;
    return rst;
}
#endif
