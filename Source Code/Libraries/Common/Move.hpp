#pragma region Include

#pragma once

#include <utility>

#include "Header.hpp"

#pragma endregion


namespace GreatVulkan
{
	template<class Type_> inline typename std::remove_reference<Type_>::type&& Move(Type_&& type_);
	template<class Type_> inline Type_&& Forward(typename std::remove_reference<Type_>::type& type_);
}


#pragma region GreatVulkan

template<class Type_>
inline typename std::remove_reference<Type_>::type&& GreatVulkan::Move(Type_&& type_)
{
	return std::move(type_);
}

template<class Type_>
inline Type_&& GreatVulkan::Forward(typename std::remove_reference<Type_>::type& type_)
{
	return std::forward<Type_>(type_);
}

#pragma endregion


#pragma region
#pragma endregion


