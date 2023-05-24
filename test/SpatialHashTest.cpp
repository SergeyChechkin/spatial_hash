/// BSD 3-Clause License
/// Copyright (c) 2023, Sergey Chechkin
/// Autor: Sergey Chechkin, schechkin@gmail.com 

#include "spatial_hash/SpatialHash2DVector.h"
#include "spatial_hash/SpatialHash3DVector.h"
#include <eigen3/Eigen/Eigen>
#include <gtest/gtest.h>
#include <random>

using namespace libs::spatial_hash;

TEST(SpatalHash2D, ConvertionTest) { 
    SpatalHash2D hash;

    uint64_t max_range = std::pow(2, 31); 
    ASSERT_EQ((max_range << 32u) + max_range, hash(HashIndex2D(0, 0))); 
    ASSERT_EQ(0, hash(HashIndex2D(-max_range, -max_range)));
    ASSERT_EQ(0, hash(HashIndex2D(max_range, max_range)));
}

TEST(SpatialHashTable2DVector, SingleSellTest) { 
    SpatialHashTable2DVector<float, size_t> hash_table(10);
    float point[2] = {0, 0};
    hash_table.Add(point, 1);
    auto cell_idx = hash_table.GetCellIndex(point);
    auto result = hash_table.GetCellData(cell_idx);
    ASSERT_EQ(1, result.size());
}

TEST(SpatialHashTable2DVector, RandomSqareTest) { 
    SpatialHashTable2DVector<float, size_t> hash_table(10);

    float square_size = 1000; 
    std::default_random_engine rng;
    std::uniform_real_distribution urd(0.0f, square_size);

    size_t size = 10000;
    for(size_t i = 0; i < size; ++i) {
        float point[2] = {urd(rng), urd(rng)};
        hash_table.Add(point, i);
    }

    float left_top[2] = {0, 0};
    float right_bottom[2] = {square_size, square_size};
    auto result = hash_table.SquareSearch(left_top, right_bottom);
    ASSERT_EQ(size, result.size());
    result = hash_table.SquareSearch(right_bottom, left_top);
    ASSERT_EQ(size, result.size());
}

TEST(SpatialHashTable2DVector, RandomCircleTest) { 
    SpatialHashTable2DVector<float, size_t> hash_table(10);
    
    float radius = 100;
    std::default_random_engine rng;
    std::uniform_real_distribution<float> r_dst(0.0, radius);
    std::uniform_real_distribution<float> angl_dst(0.0, 2*M_PI);

    size_t size = 10000;
    for(size_t i = 0; i < size; ++i) {
        float r = r_dst(rng);
        float a = angl_dst(rng);
        
        float point[2] = {r * std::cos(a), r * std::sin(a)};
        hash_table.Add(point, i);
    }

    float center[2] = {0, 0};
    auto result = hash_table.SquareSearch(center, radius);
    ASSERT_EQ(size, result.size());
}

TEST(SpatialHashTable3DVector, SingleVoxelTest) { 
    SpatialHashTable3DVector<float, size_t> hash_table(10);
    float point[3] = {0, 0, 0};
    hash_table.Add(point, 1);
    auto cell_idx = hash_table.GetVoxelIndex(point);
    auto result = hash_table.GetVoxelData(cell_idx);
    ASSERT_EQ(1, result.size());
}

TEST(SpatialHashTable3DVector, RandomCubeTest) { 
    SpatialHashTable3DVector<float, size_t> hash_table(10);

    float cube_size = 1000; 
    std::default_random_engine rng;
    std::uniform_real_distribution urd(0.0f, cube_size);

    size_t size = 100000;
    for(size_t i = 0; i < size; ++i) {
        float point[3] = {urd(rng), urd(rng), urd(rng)};
        hash_table.Add(point, i);
    }

    float p1[3] = {0, 0, 0};
    float p2[3] = {cube_size, cube_size, cube_size};
    auto result = hash_table.CubeSearch(p1, p2);

    ASSERT_EQ(size, result.size());

    result = hash_table.CubeSearch(p2, p1);
    ASSERT_EQ(size, result.size());
}

struct UnitSphereDistribution {
    Eigen::Vector3f operator()(std::default_random_engine& rng)
    {
        float angl = angl_dst_(rng);
        float z = z_dst_(rng);
        const auto scale = std::sqrt(1 - z * z);
        return {scale * std::cos(angl), scale * std::sin(angl), z};
    }

    std::uniform_real_distribution<float> angl_dst_ {0.0, 2 * M_PI};
    std::uniform_real_distribution<float> z_dst_ {-1, 1};
};

TEST(SpatialHashTable3DVector, RadiusSearchTest) { 
    // generate a point cloud
    std::vector<Eigen::Vector3f> point_cloud;

    float r1 = 1.0f;
    float r2 = 2.0f;
    float r3 = 3.0f;

    std::default_random_engine rng;
    std::uniform_real_distribution r_dst_1(0.0f, r1);
    std::uniform_real_distribution r_dst_2(r2, r3);
    UnitSphereDistribution sphere;

    size_t size_1 = 1000;
    size_t size_2 = 2000;

    for(int i = 0; i < size_1; ++i) {
        point_cloud.push_back(r_dst_1(rng) * sphere(rng));
    }

    for(int i = size_1; i < size_2; ++i) {
        point_cloud.push_back(r_dst_2(rng) * sphere(rng));
    }

    // populate hash table
    SpatialHashTable3DVector<float, size_t> hash_table(0.1f); 
    for(size_t i = 0; i < point_cloud.size(); ++i) {
        hash_table.Add(point_cloud[i].data(), i);
    }

    // peform square search 
    float radius = 1.5;
    Eigen::Vector3f center(0, 0, 0);
    auto idxs = hash_table.CubeSearch(center.data(), radius);
    
    // peform radius search
    std::vector<Eigen::Vector3f> radius_search_result;
    float radius_sqr = radius * radius;
    for(const auto& idx : idxs) {
        if ((point_cloud[idx]-center).squaredNorm() < radius_sqr) {
            radius_search_result.push_back(point_cloud[idx]);
        }
    }

    ASSERT_EQ(size_1, radius_search_result.size());
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}