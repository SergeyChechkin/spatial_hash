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
template<typename DataType, typename KeyT, typename RefType, unsigned int limit>
class SpatialHashTable2DHeap : public SpatialHashTable2D<DataType, RefType, ContainerHeap<KeyT, RefType, limit>> {
public:
    using CellType = ContainerHeap<KeyT, RefType, limit>;
    using BaseClass = SpatialHashTable2D<DataType, RefType, ContainerHeap<KeyT, RefType, limit>>;
    using HashTableType = typename BaseClass::HashTableType;
public:
    SpatialHashTable2DHeap() : BaseClass() {} 
    SpatialHashTable2DHeap(DataType cell_size) : BaseClass(cell_size) {} 

    /// @brief Add value to hash table
    /// @param point - continuous 2D space point
    /// @param ref - associated data
    void Add(const DataType point[2], KeyT key, RefType ref) {
        HashIndex2D cell_index = BaseClass::GetCellIndex(point);
        BaseClass::table_[cell_index].Add(key, ref);
    }
};

}