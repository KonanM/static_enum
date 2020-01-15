
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
//
#include <static_enum/static_enum.hpp>
#include <iostream>
#include <algorithm>

enum class Color : int { GREEN = 7, RED = -12, BLUE = 15 };


int main()
{
	constexpr auto colorEnumerators = static_enum::get_enumerators<Color>();
	//check the size of the array to get the number of different values
	static_assert(colorEnumerators.size() == 3); 

	constexpr std::array<std::string_view, 3> colorEnumeratorNames = {"RED", "GREEN", "BLUE"};
	std::array<Color, 3> colorsFromNames;
	std::transform(colorEnumeratorNames.begin(), colorEnumeratorNames.end(), colorsFromNames.begin(), [](auto& val) { return *static_enum::from_string<Color>(val); });

	for (Color e : colorEnumerators)
		std::cout << static_enum::enum_cast(e) << ": " << static_cast<int>(e) << "\n";
}

