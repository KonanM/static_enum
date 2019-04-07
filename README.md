# Static Enum C++

## What is Static Enum?

Static Enum is a single header C++17 library which provides compile time enumumeration information without using any macros or having to define the enum with some macro magic.

* `static_enum::to_string` get the name from an enum variable, returns a `constexpr std::optional<std::string_view>`
* `static_enum::from_string` get the enum variable from a string, returns a `constexpr std::optional<Enum>`
* `static_enum::get_enumerators` creates a `std::array<Enum,N>` with all enumeration values (sorted by value)

## Where is the drawback?
Static Enum uses compiler intrinsics - namely `__PRETTY_FUNCTION__` and `__FUNCSIG__` to check whether an enumeration value is valid and make the string conversions. I have  taken this trick from https://github.com/Neargye/magic_enum and adapted it slightly.  
The main drawback is that this library creates the enumerators by checking all the possible values. The default limit is to check for 256 values, which only works for arbitrary enums where the size of the underlying type is smaller than 2 bytes.
It also works for enums of bigger types, but the enumeration values have to be in the range of [-127, 128] for signed types and [0, 255] for unsigned types.


## Features

* C++17
* Single Header
* Dependency-free
* constexpr
* Works with 3rd party enums

## Example

```Cpp
enum class Color : int { GREEN = 7, RED = -12, BLUE = 15 };
// the deduced type is std::array<Color, 3>, it will update as you update the enum
constexpr auto colorEnumerators = static_enum::get_enumerators<Color>();
//check the size of the array to get the number of different values
static_assert(colorEnumerators.size() == 3); 
//you can also convert from names
constexpr std::array<std::string_view, 3> colorEnumeratorNames = {"RED", "GREEN", "BLUE"};
std::array<Color, 3> colorsFromNames;
std::transform(colorEnumeratorNames.begin(), colorEnumeratorNames.end(), colorsFromNames.begin(), [](auto& val) { return *static_enum::from_string<Color>(val); });
//or convert an enum to a name
for (Color e : colorEnumerators)
	std::cout << static_enum::to_string(e).value() << ": " << static_cast<int>(e) << "\n";
```
This will print:  
RED: -12  
GREEN: 7  
BLUE: 15  
## Compiler compatibility

* Clang/LLVM >= 5
* Visual C++ >= 15.3 / Visual Studio >= 2017
* Xcode >= 10.2
* GCC >= 9

## Licensed under the [MIT License](LICENSE)
