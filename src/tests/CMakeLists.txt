cmake_minimum_required(VERSION 3.16)

find_package(Catch2 3 REQUIRED)

add_executable(integer2string_test integer2string_test.cpp)
target_link_libraries(integer2string_test PRIVATE Catch2::Catch2WithMain schema dblayer)
add_test(NAME integer2string_test COMMAND integer2string_test)

add_executable(isFloat_test isFloat_test.cpp)
target_link_libraries(isFloat_test PRIVATE Catch2::Catch2WithMain schema dblayer)
add_test(NAME isFloat_test COMMAND isFloat_test)

add_executable(field_destructor_test field_destructor_test.cpp)
target_link_libraries(field_destructor_test PRIVATE Catch2::Catch2WithMain schema dblayer)
add_test(NAME field_destructor_test COMMAND field_destructor_test)
