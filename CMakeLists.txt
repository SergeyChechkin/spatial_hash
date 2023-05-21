cmake_minimum_required(VERSION 3.8)
project(spatial_hash_lib)

find_package(Eigen3 REQUIRED)
 
set(SOURCES
    src/SpatialHash.cpp
)

set(HEADERS
)

add_library(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_include_directories(${PROJECT_NAME} PRIVATE include)
target_include_directories(${PROJECT_NAME} PRIVATE ${EIGEN3_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${EIGEN3_LIBS})

add_subdirectory(test)