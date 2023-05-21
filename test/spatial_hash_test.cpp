/// BSD 3-Clause License
/// Copyright (c) 2023, Sergey Chechkin
/// Autor: Sergey Chechkin, schechkin@gmail.com 

#include "spatial_hash/SpatialHash2DVector.h"
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

TEST(SpatialHashTable2DVector, RandomSqareTest) { 
    SpatialHashTable2DVector<float, uint64_t> hash_table(10);

    float square_size = 1000; 
    std::default_random_engine rng;
    std::uniform_real_distribution urd(0.0f, square_size);

    size_t size = 100000;
    for(size_t i = 0; i < size; ++i) {
        float point[2] = {urd(rng), urd(rng)};
        hash_table.Add(point, i);
    }

    float left_top[2] = {0, 0};
    float  right_bottom[2] = {square_size, square_size};
    auto result = hash_table.SquareSearch(left_top, right_bottom);

    ASSERT_EQ(size, result.size());
    ASSERT_EQ(square_size * square_size / (hash_table.GetCellSize() * hash_table.GetCellSize()), hash_table.GetTable().size());

}
 
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}