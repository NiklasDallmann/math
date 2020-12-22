cmake_minimum_required(VERSION 3.15)

project(math CXX)

find_package(cxxutility REQUIRED)

include(CTest)

set(CMAKE_CXX_STANDARD				20)
set(CMAKE_CXX_STANDARD_REQUIRED		ON)
set(CMAKE_CXX_FLAGS_DEBUG			"${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=undefined -fsanitize=address -fno-omit-frame-pointer -Werror -Wall -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE			"${CMAKE_CXX_FLAGS_RELEASE} -march=native -mtune=native -O3 -Wall -Wextra")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO	${CMAKE_CXX_FLAGS_RELEASE})
#set(CMAKE_CXX_COMPILER				/usr/bin/clang++)

set(ND_MATH_INCLUDE_DIR				${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(${CMAKE_PROJECT_NAME} INTERFACE)

set(ND_MATH_DEFAULT_BUILD_TYPE "RelWithDebInfo")
if(NOT ${CMAKE_BUILD_TYPE})
	string(TOLOWER ND_MATH_BUILD_TYPE ${ND_MATH_DEFAULT_BUILD_TYPE})
else()
	string(TOLOWER ND_MATH_BUILD_TYPE ${CMAKE_BUILD_TYPE})
endif()

if(ND_MATH_BUILD_TYPE STREQUAL "release" OR ND_MATH_BUILD_TYPE STREQUAL "relwithdebinfo")
	target_compile_definitions(${CMAKE_PROJECT_NAME} INTERFACE "NDEBUG")
endif()

if(BUILD_TESTING)
	add_subdirectory(test)
endif()
