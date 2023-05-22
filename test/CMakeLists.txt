find_package(GTest REQUIRED)
find_package(Eigen3 REQUIRED)

include_directories(${GTEST_INCLUDE_DIRS})

set(TEST_PROJECT_NAME ${PROJECT_NAME}_test)
add_executable(${TEST_PROJECT_NAME} SpatialHashTest.cpp)
target_include_directories(${TEST_PROJECT_NAME} PRIVATE ../include)
target_include_directories(${PROJECT_NAME} PRIVATE ${EIGEN3_INCLUDE_DIR})
add_test(NAME ${TEST_PROJECT_NAME} COMMAND ${TEST_PROJECT_NAME})

target_link_libraries(${TEST_PROJECT_NAME} ${GTEST_LIBRARIES} ${PROJECT_NAME})