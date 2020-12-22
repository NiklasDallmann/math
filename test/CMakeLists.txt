add_executable(matrix
	${CMAKE_CURRENT_SOURCE_DIR}/matrix.cpp)
target_include_directories(matrix PRIVATE ${ND_MATH_INCLUDE_DIR})

add_executable(quaternion
	${CMAKE_CURRENT_SOURCE_DIR}/quaternion.cpp)
target_include_directories(quaternion PRIVATE ${ND_MATH_INCLUDE_DIR})

add_executable(units
	${CMAKE_CURRENT_SOURCE_DIR}/units.cpp)
target_include_directories(units PRIVATE ${ND_MATH_INCLUDE_DIR})
target_link_libraries(units PRIVATE cxxutility)

add_test(matrix_test matrix)
add_test(quaternion_test quaternion)
add_test(units_test units)
