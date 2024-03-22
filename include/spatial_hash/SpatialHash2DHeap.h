/// BSD 3-Clause License
/// Copyright (c) 2023, Sergey Chechkin
/// Autor: Sergey Chechkin, schechkin@gmail.com 

#pragma once

#include "spatial_hash/SpatialHash2D.h"
#include "spatial_hash/Containers.h"

namespace libs::spatial_hash {

/// @brief 2D spatial hash table with limited priority queue container. 
/// @tparam DataType - 2D spase data type (float, double) 
/// @tparam RefType - associated data type 
template<typename DataType, typename KeyT, typename RefType>
class SpatialHashTable2DHeap : public SpatialHashTable2D<DataType, RefType, ContainerHeap<KeyT, RefType>> {
public:
    using CellType = ContainerHeap<KeyT, RefType>;
    using BaseClass = SpatialHashTable2D<DataType, RefType, ContainerHeap<KeyT, RefType>>;
    using HashTableType = typename BaseClass::HashTableType;
public:
    SpatialHashTable2DHeap() : BaseClass() {} 
    SpatialHashTable2DHeap(DataType cell_size, size_t limit) : BaseClass(cell_size), limit_(limit) {} 

    /// @brief Add value to hash table
    /// @param point - continuous 2D space point
    /// @param ref - associated data
    void Add(const DataType point[2], KeyT key, RefType ref) {
        HashIndex2D cell_index = BaseClass::GetCellIndex(point);
        BaseClass::table_[cell_index].Add(key, ref, limit_);
    }

    std::vector<RefType> GetAllData() const {
        std::vector<RefType> result;

        for(const auto cell : BaseClass::table_) {
            for(const auto itr : cell.second) {
                result.push_back(itr.second); 
            }    
        }

        return result;
    }

private:
    size_t limit_ = std::numeric_limits<size_t>::max();
};

}