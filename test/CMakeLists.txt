#FetchContent is only available for version 3.14 and upwards and we need it to get googletest
if(${CMAKE_VERSION} VERSION_LESS "3.14.0") 
    include(FetchContent)
endif()

set(gtest_force_shared_crt ON CACHE BOOL "Always use msvcrt.dll" FORCE)
# Download and unpack googletest at configure time
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        master
)
# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
FetchContent_MakeAvailable(googletest)

add_executable(static_enum_test)
target_sources(static_enum_test PRIVATE test.cpp)
target_link_libraries(static_enum_test PRIVATE gtest gtest_main static_enum)
if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_compile_options(static_enum_test PRIVATE /W4 /permissive-)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(static_enum_test PRIVATE -Wall -Wextra -pedantic-errors)
endif()

# add our tests automatically to ctest which makes them discoverable by IDE's like MSVS
include(GoogleTest)
gtest_discover_tests(static_enum_test)