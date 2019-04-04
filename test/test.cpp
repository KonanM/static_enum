// Licensed under the MIT License <http://opensource.org/licenses/MIT>.
// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Daniil Goncharov <neargye@gmail.com>.
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
#define MAGIC_ENUM_RANGE 128
#include <static_enum/static_enum.hpp>

enum class Color { RED = -12, GREEN = 7, BLUE = 15 };

enum class Numbers : char { one = 10, two = 20, three = 30 };

enum Directions { Up = 85, Down = -42, Right = 119, Left = -119 };

enum number : int { one = 10, two = 20, three = 30 };

TEST(enum_to_string, enum_to_string_color_number)
{
  Color cr = Color::RED;
  static Color cm[3] = {Color::RED, Color::GREEN, Color::BLUE};
  ASSERT_TRUE(magic_enum::enum_to_string<Color::RED>().value() == "RED");
  ASSERT_TRUE(magic_enum::enum_to_string<Color::BLUE>().value() == "BLUE");
  //ASSERT_TRUE(magic_enum::enum_to_string<cm[1]>().value() == "GREEN");

}

TEST(enum_to_string, enum_to_string_2)
{
	ASSERT_EQ(magic_enum::enum_to_string(Color::RED).value(), "RED");
	ASSERT_TRUE(!magic_enum::enum_to_string(static_cast<Color>(5)));
}

TEST(enum_to_string, enum_to_string_from_string)
{
  
  ASSERT_TRUE(magic_enum::enum_from_string<Color>("RED").value() == Color::RED);
  ASSERT_TRUE(magic_enum::enum_from_string<Color>("GREEN").value() == Color::GREEN);
  ASSERT_TRUE(magic_enum::enum_from_string<Color>("BLUE").value() == Color::BLUE);
  ASSERT_TRUE(!magic_enum::enum_from_string<Color>("None").has_value());

  //ASSERT_TRUE(magic_enum::enum_from_string<Numbers>("one").value() == Numbers::one);
  //ASSERT_TRUE(magic_enum::enum_from_string<Numbers>("two").value() == Numbers::two);
  //ASSERT_TRUE(magic_enum::enum_from_string<Numbers>("two").value() == Numbers::two);
  //ASSERT_TRUE(!magic_enum::enum_from_string<Numbers>("None").has_value());

  ASSERT_TRUE(magic_enum::enum_from_string<Directions>("Up").value() == Directions::Up);
  ASSERT_TRUE(magic_enum::enum_from_string<Directions>("Down").value() == Directions::Down);
  ASSERT_TRUE(magic_enum::enum_from_string<Directions>("Right").value() == Directions::Right);
  ASSERT_TRUE(magic_enum::enum_from_string<Directions>("Left").value() == Directions::Left);
  ASSERT_TRUE(!magic_enum::enum_from_string<Directions>("None").has_value());

  ASSERT_TRUE(magic_enum::enum_from_string<number>("one").value() == number::one);
  ASSERT_TRUE(magic_enum::enum_from_string<number>("two").value() == number::two);
  ASSERT_TRUE(magic_enum::enum_from_string<number>("two").value() == number::two);
  ASSERT_TRUE(!magic_enum::enum_from_string<number>("None").has_value());
}
