#ifndef _TYPE_TRAITER_HPP
#define _TYPE_TRAITER_HPP
/* module for TypeTraiter
 *
 * used boost/type traits
 *
 * Gaohan
 */
#include <string>
#include <deque>
#include <list>
#include <vector>
#include <forward_list>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <typeindex>
#include <type_traits>


namespace type_traiter{
	using namespace std;

	template<typename T, typename ... Types>
		struct is_string {
			static constexpr bool value = false;
		};

	template<typename ... Types>
		struct is_string<basic_string<Types...> > {
			static constexpr bool value = true;
		};


	template<typename T, typename ... Types>
		struct is_deque {
			static constexpr bool value = false;
		};

	template<typename ... Types>
		struct is_deque<deque<Types...> > {
			static constexpr bool value = true;
		};


	template<typename T, typename ... Types>
		struct is_forward_list {
			static constexpr bool value = false;
		};

	template<typename ... Types>
		struct is_forward_list<forward_list<Types...> > {
			static constexpr bool value = true;
		};


	template<typename T, typename ... Types>
		struct is_list {
			static constexpr bool value = false;
		};

	template<typename ... Types>
		struct is_list<list<Types...> > {
			static constexpr bool value = true;
		};


	template<typename T, typename ... Types>
		struct is_vector {
			static constexpr bool value = false;
		};

	template<typename ... Types>
		struct is_vector<vector<Types...> > {
			static constexpr bool value = true;
		};


	template<typename T, typename ... Types>
		struct is_map {
			static constexpr bool value = false;
		};

	template<typename ... Types>
		struct is_map<map<Types...> > {
			static constexpr bool value = true;
		};


	template<typename T, typename ... Types>
		struct is_set {
			static constexpr bool value = false;
		};

	template<typename ... Types>
		struct is_set<set<Types...> > {
			static constexpr bool value = true;
		};


	template<typename T, typename ... Types>
		struct is_unordered_map {
			static constexpr bool value = false;
		};

	template<typename ... Types>
		struct is_unordered_map<unordered_map<Types...> > {
			static constexpr bool value = true;
		};


	template<typename T, typename ... Types>
		struct is_unordered_set {
			static constexpr bool value = false;
		};

	template<typename ... Types>
		struct is_unordered_set<unordered_set<Types...> > {
			static constexpr bool value = true;
		};


	template <typename T>
		struct is_sequence_container {
			static constexpr bool value
				=  is_deque<T>::value
				|| is_forward_list<T>::value
				|| is_list<T>::value
				|| is_vector<T>::value;
		};


	template <typename T>
		struct is_associative_container {
			static constexpr bool value
				=  is_map<T>::value
				|| is_set<T>::value;
		};


	template <typename T>
		struct is_unordered_associative_container {
			static constexpr bool value
				=  is_unordered_map<T>::value
				|| is_unordered_set<T>::value;
		};


	template <typename T>
		struct is_container {
			static constexpr bool value
				=  is_sequence_container<T>::value
				|| is_associative_container<T>::value
				|| is_unordered_associative_container<T>::value;
		};
};

#endif
