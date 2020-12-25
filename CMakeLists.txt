cmake_minimum_required(VERSION 3.15)

project(ndmath
	LANGUAGES	CXX
	VERSION		0.1.0)

find_package(cxxutility REQUIRED)

include(CTest)

set(CMAKE_CXX_STANDARD				20)
set(CMAKE_CXX_STANDARD_REQUIRED		ON)

# Set appropriate compiler flags
if(UNIX)
	set(CMAKE_CXX_FLAGS_DEBUG				"-fsanitize=undefined -fsanitize=address -fno-omit-frame-pointer -pedantic-errors -Wpedantic -Werror -Wall -Wextra")
	set(CMAKE_CXX_FLAGS_RELEASE				"-march=native -mtune=native -O3 -Wall -Wextra")
	set(CMAKE_CXX_FLAGS_RELWITHDEBINFO		"${CMAKE_CXX_FLAGS_RELEASE} -g")
elseif (WIN32)
	if(MSVC)
		set(CMAKE_CXX_FLAGS_DEBUG			"/DEBUG:FULL")
		set(CMAKE_CXX_FLAGS_RELEASE			"/O2 /arch:AVX2")
		set(CMAKE_CXX_FLAGS_RELWITHDEBINFO	"${CMAKE_CXX_FLAGS_RELEASE} /DEBUG")
	else()
		message("Win32 possibly with Clang")
	endif()
else()
	message(FATAL_ERROR "Unsupported platform")
endif()

set(ND_MATH_INCLUDE_DIR						"${CMAKE_CURRENT_SOURCE_DIR}/include")

# Library target
add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME}
	INTERFACE								$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
											$<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include>)

# Set compile definitions dependent on build type
set(ND_MATH_DEFAULT_BUILD_TYPE "RelWithDebInfo")
if(NOT ${CMAKE_BUILD_TYPE})
	string(TOLOWER ND_MATH_BUILD_TYPE "${ND_MATH_DEFAULT_BUILD_TYPE}")
else()
	string(TOLOWER ND_MATH_BUILD_TYPE "${CMAKE_BUILD_TYPE}")
endif()

if(ND_MATH_BUILD_TYPE STREQUAL "release" OR ND_MATH_BUILD_TYPE STREQUAL "relwithdebinfo")
	target_compile_definitions(${PROJECT_NAME} INTERFACE "NDEBUG")
endif()

# Tests
if(BUILD_TESTING)
	add_subdirectory(test)
endif()

# Package
set(CPACK_PACKAGE_NAME							${PROJECT_NAME})
set(CPACK_PACKAGE_VENDOR						"Niklas Dallmann & Adrian Kulisch")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY			"Typed linear algebra and unit library")
set(CPACK_PACKAGE_VERSION						${PROJECT_VERSION})
set(CPACK_PACKAGE_VERSION_MAJOR					${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR					${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH					${PROJECT_VERSION_PATCH})
set(CPACK_RESOURCE_FILE_LICENSE					"${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README					"${CMAKE_CURRENT_SOURCE_DIR}/README.md")
set(CPACK_SOURCE_IGNORE_FILES
	"/.git"
	"/bin"
	"/lib"
	"/test"
	".gitignore"
	"CMakeLists.txt.user"
	"CMakeLists.txt.autosave")

if(UNIX)
	set(CPACK_SOURCE_GENERATOR					"TGZ")
elseif(MSVC)
	set(CPACK_SOURCE_GENERATOR					"NSIS")
	set(CPACK_PACKAGE_INSTALL_DIRECTORY			${PROJECT_NAME})
endif()

# Installation
set(ND_MATH_TARGETS								${PROJECT_NAME})

if(UNIX)
	set(ND_MATH_LIBRARY_DESTINATION				"${CMAKE_INSTALL_PREFIX}/lib")
	set(ND_MATH_HEADER_DESTINATION				"${CMAKE_INSTALL_PREFIX}/include/${PROJECT_NAME}")
	set(ND_MATH_CMAKE_DIRECTORY					"${CMAKE_INSTALL_PREFIX}/lib/cmake/${PROJECT_NAME}")
elseif(MSVC)
	set(ND_MATH_LIBRARY_DESTINATION				"lib")
	set(ND_MATH_HEADER_DESTINATION				"include/${PROJECT_NAME}")
	set(ND_MATH_CMAKE_DIRECTORY					"lib/cmake/${PROJECT_NAME}")
endif()

install(TARGETS									${PROJECT_NAME}
		EXPORT									${ND_MATH_TARGETS}
		INCLUDES DESTINATION					"${ND_MATH_HEADER_DESTINATION}")

install(DIRECTORY								"include/"
		DESTINATION								"${ND_MATH_HEADER_DESTINATION}")

install(EXPORT									${ND_MATH_TARGETS}
		DESTINATION								${ND_MATH_CMAKE_DIRECTORY}
		FILE									"ndmath-config.cmake")

include(CPack)
