#pragma region Include

#pragma once

#include <array>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <initializer_list>

#include "Header.hpp"
#include "Size.hpp"

#pragma endregion


namespace GreatVulkan
{
	template<class Type_, Size size_>	using Array			= std::array<Type_, size_>;
	template<class Type_>				using Vector		= std::vector<Type_>;
	template<class Type_>				using List			= std::list<Type_>;
	template<class Key_, class Value_>	using Map			= std::map<Key_, Value_>;
	template<class Type_>				using Set			= std::set<Type_>;
	template<class Type_>				using Initializer	= std::initializer_list<Type_>;


	template <typename T>
	Vector<T> operator + (const Vector<T>& a_, const Vector<T>& b_);
	template <typename T>
	Vector<T> operator + (const Vector<T>& a_, const T& b_);
}


#pragma region GreatVulkan

template <typename T>
GreatVulkan::Vector<T> GreatVulkan::operator + (const Vector<T>& a_, const Vector<T>& b_)
{
	Vector<T> ab;

	ab.reserve(a_.size() + b_.size());
	ab.insert(ab.end(), a_.begin(), a_.end());
	ab.insert(ab.end(), b_.begin(), b_.end());

	return Move(ab);
}
template <typename T>
GreatVulkan::Vector<T> GreatVulkan::operator + (const Vector<T>& a_, const T& b_)
{
	Vector<T> ab;

	ab.reserve(a_.size() + 1);
	ab.insert(ab.end(), a_.begin(), a_.end());
	ab.push_back(b_);

	return Move(ab);
}

#pragma endregion


#pragma region
#pragma endregion


