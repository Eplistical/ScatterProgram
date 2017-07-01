#ifndef _TYPE_TRAITER_HPP
#define _TYPE_TRAITER_HPP
/* module for TypeTraiter
 *
 * used boost/type traits
 *
 * Gaohan
 */
#include <vector>
#include <map>
#include <set>
#include <list>
#include <string>
#include <typeindex>
#include <type_traits>


namespace type_traiter{
	using namespace std;
	// is_string
	template <typename T> struct is_string { static const bool value = false;  };
	template <typename T, typename traits, typename Alloc>
		struct is_string<basic_string<T, traits, Alloc> > { static const bool value = true; };

	// is_vector
	template <typename T> struct is_vector { static const bool value = false;  };
	template <typename T, typename A> struct is_vector<vector<T, A> > { static const bool value = true; };

};

#endif
