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

#include <gtest/gtest.h>
#define STATIC_ENUM_RANGE 256
#include <static_enum/static_enum.hpp>

enum class Simple : unsigned { ONE, TWO, THREE };

enum Directions { Up = 85, Down = -42, Right = 119, Left = -119 };

enum number : int { one = 10, two = 20, three = 30 };

enum class Numbers : unsigned char { one = 10, two = 20, three = 30 };

enum class Color : int { GREEN = 7, RED = -12, BLUE = 15 };

enum class Alphabet : size_t { a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z };

TEST(static_enum, enum_range_constexpr)
{
	constexpr auto enumRangeAlphabet = static_enum::get_enumerators<Alphabet>();
	static_assert(enumRangeAlphabet.size() == 26);

	for (size_t i = 0; i < 26; ++i)
		ASSERT_EQ(static_enum::to_string(enumRangeAlphabet[i]).front(), ('a' + i));
	
	constexpr auto enumRangeColor = static_enum::get_enumerators<Color>();
	static_assert(enumRangeColor.size() == 3);
	static_assert(enumRangeColor[0] == Color::RED, "Color must match");
	static_assert(enumRangeColor[1] == Color::GREEN, "Color must match");
	static_assert(enumRangeColor[2] == Color::BLUE, "Color must match");

	constexpr auto enumRangeDirections = static_enum::get_enumerators<Directions>();
	static_assert(enumRangeDirections.size() == 4);
	static_assert(enumRangeDirections[0] == Directions::Left, "Direction must match");
	static_assert(enumRangeDirections[1] == Directions::Down, "Direction must match");
	static_assert(enumRangeDirections[2] == Directions::Up, "Direction must match");
	static_assert(enumRangeDirections[3] == Directions::Right, "Direction must match");

	constexpr auto enumRangeSimple = static_enum::get_enumerators<Simple>();

	static_assert(enumRangeSimple[0] == Simple::ONE, "Number must match");
	static_assert(enumRangeSimple[1] == Simple::TWO, "Number must match");
	static_assert(enumRangeSimple[2] == Simple::THREE, "Number must match");
}

TEST(static_enum, to_string_dynamic)
{
	ASSERT_TRUE(static_enum::to_string<Color>(Color::RED) == "RED");
	ASSERT_TRUE(static_enum::to_string(Color::BLUE) == "BLUE");
	ASSERT_TRUE(static_enum::to_string(Color::GREEN) == "GREEN");
	ASSERT_TRUE(static_enum::to_string(static_cast<Color>(4)).empty());

	ASSERT_TRUE(static_enum::to_string(Numbers::one) == "one");
	ASSERT_TRUE(static_enum::to_string(Numbers::two) == "two");
	ASSERT_TRUE(static_enum::to_string(Numbers::three) == "three");
	ASSERT_TRUE(static_enum::to_string(static_cast<Numbers>(4)).empty());

}

TEST(static_enum, to_string_static)
{
	ASSERT_TRUE(static_enum::to_string<Color::RED>() == "RED");
	ASSERT_TRUE(static_enum::to_string<Color::BLUE>() == "BLUE");
	ASSERT_TRUE(static_enum::to_string<Color::GREEN>() == "GREEN");
	ASSERT_TRUE(static_enum::to_string<static_cast<Color>(4)>().empty());

	ASSERT_TRUE(static_enum::to_string<Numbers::one>() == "one");
	ASSERT_TRUE(static_enum::to_string<Numbers::two>() == "two");
	ASSERT_TRUE(static_enum::to_string<Numbers::three>() == "three");
	ASSERT_TRUE(static_enum::to_string<static_cast<Numbers>(4)>().empty());

	ASSERT_TRUE(static_enum::to_string<Directions::Left>() == "Left");
	ASSERT_TRUE(static_enum::to_string<Directions::Right>() == "Right");
	ASSERT_TRUE(static_enum::to_string<Directions::Up>() == "Up");
	ASSERT_TRUE(static_enum::to_string<Directions::Down>() == "Down");
	ASSERT_TRUE(static_enum::to_string<static_cast<Directions>(123)>().empty());
}

TEST(static_enum, from_string)
{
  ASSERT_TRUE(static_enum::from_string<Color>("RED").value() == Color::RED);
  ASSERT_TRUE(static_enum::from_string<Color>("GREEN").value() == Color::GREEN);
  ASSERT_TRUE(static_enum::from_string<Color>("BLUE").value() == Color::BLUE);
  ASSERT_TRUE(!static_enum::from_string<Color>("None").has_value());

  ASSERT_TRUE(static_enum::from_string<Numbers>("one").value() == Numbers::one);
  ASSERT_TRUE(static_enum::from_string<Numbers>("two").value() == Numbers::two);
  ASSERT_TRUE(static_enum::from_string<Numbers>("two").value() == Numbers::two);
  ASSERT_TRUE(!static_enum::from_string<Numbers>("None").has_value());

  ASSERT_TRUE(static_enum::from_string<Directions>("Up").value() == Directions::Up);
  ASSERT_TRUE(static_enum::from_string<Directions>("Down").value() == Directions::Down);
  ASSERT_TRUE(static_enum::from_string<Directions>("Right").value() == Directions::Right);
  ASSERT_TRUE(static_enum::from_string<Directions>("Left").value() == Directions::Left);
  ASSERT_TRUE(!static_enum::from_string<Directions>("None").has_value());

  ASSERT_TRUE(static_enum::from_string<number>("one").value() == number::one);
  ASSERT_TRUE(static_enum::from_string<number>("two").value() == number::two);
  ASSERT_TRUE(static_enum::from_string<number>("two").value() == number::two);
  ASSERT_TRUE(!static_enum::from_string<number>("None").has_value());
}

TEST(static_enum, enum_cast)
{
	ASSERT_TRUE(static_enum::enum_cast<Color::RED>() == "RED");
	ASSERT_TRUE(static_enum::enum_cast<Color::BLUE>() == "BLUE");
	ASSERT_TRUE(static_enum::enum_cast<Color::GREEN>() == "GREEN");
	ASSERT_TRUE(static_enum::enum_cast<static_cast<Color>(4)>().empty());

	ASSERT_TRUE(static_enum::enum_cast(Color::RED) == "RED");
	ASSERT_TRUE(static_enum::enum_cast(Color::BLUE) == "BLUE");
	ASSERT_TRUE(static_enum::enum_cast(Color::GREEN) == "GREEN");
	ASSERT_TRUE(static_enum::enum_cast(static_cast<Color>(4)).empty());

	ASSERT_TRUE(static_enum::enum_cast<Color>("RED") == Color::RED);
	ASSERT_TRUE(static_enum::enum_cast<Color>("BLUE") == Color::BLUE);
	ASSERT_TRUE(static_enum::enum_cast<Color>("GREEN") == Color::GREEN);
	ASSERT_TRUE(!static_enum::enum_cast<Color>("NotSoGreen"));
}