
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

// Enum variable must be in range (-MAGIC_ENUM_RANGE, MAGIC_ENUM_RANGE). If you need a larger range, redefine the macro MAGIC_ENUM_RANGE.
#if !defined(MAGIC_ENUM_RANGE)
#  define MAGIC_ENUM_RANGE 300000
#endif

namespace magic_enum 
{
static_assert(MAGIC_ENUM_RANGE > 0,
              "MAGIC_ENUM_RANGE must be positive and greater than zero.");
static_assert(MAGIC_ENUM_RANGE % 8 == 0,
              "MAGIC_ENUM_RANGE must be a multiple of 8.");
static_assert(MAGIC_ENUM_RANGE < std::numeric_limits<int>::max(),
              "MAGIC_ENUM_RANGE must be less INT_MAX.");

namespace detail 
{

template <typename E, E V>
[[nodiscard]] constexpr std::optional<std::string_view> enum_to_string_impl() noexcept 
{
	static_assert(std::is_enum_v<E>, "magic_enum::enum_to_string require enum type.");
#if defined(__clang__) || (defined(__GNUC__) && __GNUC__ >= 9)
	std::string_view name{ __PRETTY_FUNCTION__ };
	constexpr auto suffix = sizeof("]") - 1;
#elif defined(_MSC_VER)
	std::string_view name{ __FUNCSIG__ };
	constexpr auto suffix = sizeof(">(void) noexcept") - 1;
#else
	return std::nullopt; // Unsupported compiler.
#endif

	std::size_t prefix = name.find_last_of(", :", name.size() - suffix) + 1;

	if (name[prefix] != '0') {
		return name.substr(prefix , name.size() - suffix - prefix);
	}
	else {
		return std::nullopt; // Enum variable does not have name.
	}
}

template <typename E, int V>
struct enum_from_string_impl_t final 
{
  [[nodiscard]] constexpr std::optional<E> operator()(std::string_view name) const noexcept {
    if constexpr (V > std::numeric_limits<std::underlying_type_t<E>>::max()) {
      return std::nullopt; // Enum variable out of range.
    }

    if (enum_to_string_impl<E, static_cast<E>(V)>() == name) {
      return static_cast<E>(V);
    } else if (enum_to_string_impl<E, static_cast<E>(V + 1)>() == name) {
      return static_cast<E>(V + 1);
    } else if (enum_to_string_impl<E, static_cast<E>(V + 2)>() == name) {
      return static_cast<E>(V + 2);
    } else if (enum_to_string_impl<E, static_cast<E>(V + 3)>() == name) {
      return static_cast<E>(V + 3);
    } else if (enum_to_string_impl<E, static_cast<E>(V + 4)>() == name) {
      return static_cast<E>(V + 4);
    } else if (enum_to_string_impl<E, static_cast<E>(V + 5)>() == name) {
      return static_cast<E>(V + 5);
    } else if (enum_to_string_impl<E, static_cast<E>(V + 6)>() == name) {
      return static_cast<E>(V + 6);
    } else if (enum_to_string_impl<E, static_cast<E>(V + 7)>() == name) {
      return static_cast<E>(V + 7);
    } else {
      return enum_from_string_impl_t<E, V + 8>{}(name);
    }
  }
};

template <typename E, typename U, U ...I>
[[nodiscard]] constexpr std::optional<E> enum_from_string_impl(std::string_view name, std::integer_sequence<U, I...>) noexcept
{
	std::optional<E> returnValue;
	(((returnValue = (enum_to_string_impl<E, static_cast<E>(I - MAGIC_ENUM_RANGE)>() == name) ? std::optional<E>(static_cast<E>(I - MAGIC_ENUM_RANGE)) : std::nullopt), returnValue) || ...);

	return returnValue;
};

template <typename E>
struct enum_from_string_impl_t<E, MAGIC_ENUM_RANGE> final 
{
  [[nodiscard]] constexpr std::optional<E> operator()(std::string_view) const noexcept 
  {
    static_assert(std::is_enum_v<E>, "magic_enum::enum_from_string require enum type.");
    return std::nullopt; // Enum variable out of range MAGIC_ENUM_RANGE.
  }
};

template <typename E, typename U, U ...I>
[[nodiscard]] constexpr std::optional<std::string_view> enum_to_string_impl(E value, std::integer_sequence<U, I...>) noexcept
{
	std::optional<std::string_view> returnValue;
	bool isValid = false;
	((((isValid = (value == static_cast<E>(I - MAGIC_ENUM_RANGE)))), (returnValue = isValid ? enum_to_string_impl<E, static_cast<E>(I - MAGIC_ENUM_RANGE)>() : std::nullopt), isValid) || ...);
	 
	return returnValue;
};

} // namespace detail

// enum_to_string(enum) obtains string enum name from enum variable.
template <typename E, typename U = std::underlying_type_t<std::decay_t<E>>, typename Indices = std::make_integer_sequence<U, std::is_signed_v<U> ?  2 * MAGIC_ENUM_RANGE - 1 : MAGIC_ENUM_RANGE>>
[[nodiscard]] constexpr std::optional<std::string_view> enum_to_string(E value) noexcept 
{
	//static_cast(!std::is_enum_v<std::decay_t<E>>, "Parameter has to be an enum.");
	if (static_cast<int>(value) >= MAGIC_ENUM_RANGE || static_cast<int>(value) <= -MAGIC_ENUM_RANGE)
	{
		return std::nullopt; // Enum variable out of range MAGIC_ENUM_RANGE.
	}
	return detail::enum_to_string_impl<std::decay_t<E>, U>(value, Indices{});
}

// enum_to_string<enum>() obtains string enum name from static storage enum variable.
template <auto V, typename = std::enable_if_t<std::is_enum_v<std::decay_t<decltype(V)>>>>
[[nodiscard]] constexpr std::optional<std::string_view> enum_to_string() noexcept 
{
  return detail::enum_to_string_impl<decltype(V), V>();
}

// enum_from_string(name) obtains enum variable from enum string name.
template <typename E , typename U = std::underlying_type_t<std::decay_t<E>>, typename Indices = std::make_integer_sequence<U, std::is_signed_v<U> ? 2 * MAGIC_ENUM_RANGE - 1 : MAGIC_ENUM_RANGE>>
[[nodiscard]] constexpr std::optional<E> enum_from_string(std::string_view name) noexcept 
{
	return detail::enum_from_string_impl<E, U>(name, Indices{});
}

} // namespace magic_enum
