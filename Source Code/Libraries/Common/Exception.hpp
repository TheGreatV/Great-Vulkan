#pragma region Include
#pragma once

#include "Header.hpp"
#include "String.hpp"

#pragma endregion


namespace GreatVulkan
{
	class Exception
	{
	public:
		using Text = String;
	protected:
		const Text text = "Unknown exception";
	public:
		inline Exception() = default;
		inline Exception(const Text& errorText_);
		inline Exception(const Exception&) = default;
		inline ~Exception() = default;
	public:
		inline Exception& operator = (const Exception& source) = delete;
	public:
		inline Text GetText() const;
	};
}

#pragma region GreatVulkan

#pragma region Exception

inline GreatVulkan::Exception::Exception(const Text& errorText_) :
	text(errorText_)
{
}

inline GreatVulkan::Exception::Text GreatVulkan::Exception::GetText() const
{
	return text;
}

#pragma endregion

#pragma endregion

#pragma region
#pragma endregion
