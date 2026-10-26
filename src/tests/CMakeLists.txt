cmake_minimum_required(VERSION 2.8)
project(dblayer_tests)

# Use system-installed Catch2
find_package(Catch2)

# Add test executables for the 3 P0 fixes
add_executable(integer2string_test tests/integer2string_test.cpp)
target_link_libraries(integer2string_test Catch2::Catch2 schema dblayer)

add_executable(isFloat_test tests/isFloat_test.cpp)
target_link_libraries(isFloat_test Catch2::Catch2 schema dblayer)

add_executable(field_destructor_test tests/field_destructor_test.cpp)
target_link_libraries(field_destructor_test Catch2::Catch2 schema dblayer)