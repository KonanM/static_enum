
// Licensed under the MIT License <http://opensource.org/licenses/MIT>.
// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Kinan Mahdi
//
// Permission is hereby  granted, free of charge, to any  person obtaining a copy
// of this software and associated  documentation files (the "Software"), to deal
// in the Software  without restriction, including without  limitation the rights
// to  use, copy,  modify, merge,  publish, distribute,  sublicense, and/or  sell
// copies  of  the Software,  and  to  permit persons  to  whom  the Software  is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE  IS PROVIDED "AS  IS", WITHOUT WARRANTY  OF ANY KIND,  EXPRESS OR
// IMPLIED,  INCLUDING BUT  NOT  LIMITED TO  THE  WARRANTIES OF  MERCHANTABILITY,
// FITNESS FOR  A PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN NO EVENT  SHALL THE
// AUTHORS  OR COPYRIGHT  HOLDERS  BE  LIABLE FOR  ANY  CLAIM,  DAMAGES OR  OTHER
// LIABILITY, WHETHER IN AN ACTION OF  CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE  OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#include <cstddef>
#include <type_traits>
#include <limits>
#include <string_view>
#include <optional>
#include <array>

// Enum variable must be in range (-STATIC_ENUM_RANGE / 2, STATIC_ENUM_RANGE /2) for signed types
// Enum variable must be in range (0, STATIC_ENUM_RANGE) for unsigned types
// If you need a larger range, redefine the macro STATIC_ENUM_RANGE.
#if !defined(STATIC_ENUM_RANGE)
#  define STATIC_ENUM_RANGE 256
#endif

namespace static_enum
{
	static_assert(STATIC_ENUM_RANGE > 0, "STATIC_ENUM_RANGE must be positive and greater than zero.");
	static_assert(STATIC_ENUM_RANGE < std::numeric_limits<int>::max(), "STATIC_ENUM_RANGE must be less INT_MAX.");

	namespace detail
	{
#if defined(__clang__) || (defined(__GNUC__) && __GNUC__ >= 9)
		constexpr auto suffix = sizeof("]") - 1;
#define STATIC_ENUM_FUNCSIG __PRETTY_FUNCTION__
#elif defined(_MSC_VER)
		constexpr auto suffix = sizeof(">(void) noexcept") - 1;
#define STATIC_ENUM_FUNCSIG __FUNCSIG__
#else
#define STATIC_ENUM_FUNCSIG
#error Unsupported compiler
#endif
		[[nodiscard]] constexpr bool is_digit(char c) noexcept
		{
			return c >= '0' && c <= '9';
		}
		//this is the hack that we're gonna use
		//we use either __PRETTY_FUNCTION__ or __FUNCSIG__ to analyze the function signature
		//if the enum value doesn't exist it will appear as a number
		//otherwise it will have the correct enum name for the value V
		//to check whether the enum is valid it's enough to check simply if the first letter of the enum is a number
		//this works with MSVC, gcc and clang
		//for the string conversion we simply have to extract the correct substring from the function signature
		template <typename E, E V>
		[[nodiscard]] constexpr std::string_view to_string_impl_static() noexcept
		{
			constexpr std::string_view name{ STATIC_ENUM_FUNCSIG };
			constexpr std::size_t prefix = name.find_last_of(", :)-", name.size() - suffix) + 1;

			if constexpr (!is_digit(name[prefix]))
				return name.substr(prefix, name.size() - suffix - prefix);
			else
				return {};
		}

		template <typename E, int Offset, int ...I>
		[[nodiscard]] constexpr std::optional<E> enum_from_string_impl(std::string_view name, std::integer_sequence<int, I...>) noexcept
		{
			std::optional<E> returnValue;
			(((to_string_impl_static<E, static_cast<E>(I - Offset)>() == name) ? (returnValue = static_cast<E>(I - Offset), false) : true) && ...);
			return returnValue;
		}

		template <typename E, int Offset, int ...I>
		[[nodiscard]] constexpr std::string_view to_string_impl(E value, std::integer_sequence<int, I...>) noexcept
		{
			//we have to convert the runtime value to a compile time index
			//this method uses an O(1) lookup via function pointers
			using ToStringFunctionDecl = decltype(&to_string_impl_static<E, static_cast<E>(0)>);
			constexpr std::array<ToStringFunctionDecl, sizeof...(I)> to_string_functions{ { to_string_impl_static<E, static_cast<E>(I - Offset)>... } };
			return to_string_functions[Offset + static_cast<int>(value)]();
		}

		template <typename E, int Offset, int ...I>
		[[nodiscard]] constexpr decltype(auto) get_enumerators_impl(std::integer_sequence<int, I...>) noexcept
		{
			constexpr size_t N = sizeof...(I);
			//here we create an array of bool where each index indicates whether it belongs to a valid enum entry
			constexpr std::array<bool, N> validIndices{ {!to_string_impl_static<E, static_cast<E>(I - Offset)>().empty()...} };
			//here we count the number of valid enum indices
			constexpr size_t numValid = ((validIndices[I] ? 1 : 0) + ...);
			//with this information we can build an array of only valid enum entries
			std::array<E, numValid> enumArray{};
			size_t enumIdx = 0;
			for (size_t i = 0; i < N && enumIdx < numValid; ++i)
				if (validIndices[i])
					enumArray[enumIdx++] = static_cast<E>(i - Offset);

			return enumArray;
		}

		//for small values like char we should check for the right range - we should not stress the compiler more than necessary
		template <typename U>
		struct Limit final
		{
			static constexpr int Range = sizeof(U) <= 2 ? (int)std::numeric_limits<U>::max() - (int)std::numeric_limits<U>::min() + 1 : std::numeric_limits<int>::max();
			static constexpr int Size = std::min(Range, STATIC_ENUM_RANGE);
			static constexpr int Offset = std::is_signed_v<U> ? (Size + 1) / 2 : 0;
		};

	} // namespace detail

	//get_enumerators(): return a std::array<Enum,N> with all enumeration values (sorted by value)
	template <typename E, typename U = std::underlying_type_t<std::decay_t<E>>>
	[[nodiscard]] constexpr decltype(auto) get_enumerators() noexcept
	{
		using Indices = std::make_integer_sequence<int, detail::Limit<U>::Size>;
		static_assert(std::is_enum_v<std::decay_t<E>>, "static_enum::to_string requires an enum type.");
		return detail::get_enumerators_impl<E, detail::Limit<U>::Offset>(Indices{});
	}

	//to_string(Enum): get the name from an enum variable, returns a constexpr std::string_view
	template <typename E, typename U = std::underlying_type_t<std::decay_t<E>>>
	[[nodiscard]] constexpr std::string_view to_string(E value) noexcept
	{
		using Indices = std::make_integer_sequence<int, detail::Limit<U>::Size>;
		static_assert(std::is_enum_v<std::decay_t<E>>, "static_enum::to_string requires enum type.");
		if (static_cast<int>(value) >= STATIC_ENUM_RANGE || static_cast<int>(value) <= -STATIC_ENUM_RANGE)
		{
			return {}; // Enum variable out of STATIC_ENUM_RANGE.
		}
		return detail::to_string_impl<std::decay_t<E>, detail::Limit<U>::Offset>(value, Indices{});
	}

	// to_string<Enum>(): get the name from an enum variable, prefer this over the to_string() version if possible, since
	// this version is much lighter on the compile times and is not restricted to the size limitation
	template <auto V>
	[[nodiscard]] constexpr std::string_view to_string() noexcept
	{
		static_assert(std::is_enum_v<std::decay_t<decltype(V)>>, "static_enum::to_string requires an enum type.");
		return detail::to_string_impl_static<decltype(V), V>();
	}

	//from_string(name): get the enum variable from a string, returns a constexpr std::optional<Enum>
	template <typename E, typename U = std::underlying_type_t<std::decay_t<E>>>
	[[nodiscard]] constexpr std::optional<E> from_string(std::string_view name) noexcept
	{
		using Indices = std::make_integer_sequence<int, detail::Limit<U>::Size>;
		static_assert(std::is_enum_v<std::decay_t<E>>, "static_enum::to_string requires an enum type.");
		return detail::enum_from_string_impl<E, detail::Limit<U>::Offset>(name, Indices{});
	}

	template <typename E>
	[[nodiscard]] constexpr std::optional<E> enum_cast(std::string_view name) noexcept
	{
		return from_string<E>(name);
	}

	template <auto V>
	[[nodiscard]] constexpr std::string_view enum_cast() noexcept
	{
		static_assert(std::is_enum_v<std::decay_t<decltype(V)>>, "static_enum::to_string requires an enum type.");
		return detail::to_string_impl_static<decltype(V), V>();
	}

	//to_string(Enum): get the name from an enum variable, returns a constexpr std::string_view
	template <typename E, typename U = std::underlying_type_t<std::decay_t<E>>>
	[[nodiscard]] constexpr std::string_view enum_cast(E value) noexcept
	{
		return to_string<E>(value);
	}

} // namespace static_enum
