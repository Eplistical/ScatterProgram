#ifndef _ENUMSPACE_HPP
#define _ENUMSPACE_HPP
#include <type_traits>

namespace enumspace 
{
	// -- for range-based loop on enum class -- //
	template <class EnumClass>
	typename std::enable_if<std::is_enum<EnumClass>::value, EnumClass>::type
	begin(EnumClass E) 
	{
		return EnumClass::First;
	}

	template <class EnumClass>
	typename std::enable_if<std::is_enum<EnumClass>::value, EnumClass>::type
	end(EnumClass E) 
	{
		return E = EnumClass(static_cast<typename std::underlying_type<EnumClass>::type>(EnumClass::Last) + 1);
	}

	template <class EnumClass>
	typename std::enable_if<std::is_enum<EnumClass>::value, EnumClass>::type
	operator++(EnumClass& E) 
	{
		return E = EnumClass(static_cast<typename std::underlying_type<EnumClass>::type>(E) + 1);
	}

	template <class EnumClass>
	typename std::enable_if<std::is_enum<EnumClass>::value, EnumClass>::type
	operator*(EnumClass E) 
	{
		return E;
	}
}

#endif
