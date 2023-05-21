/// BSD 3-Clause License
/// Copyright (c) 2023, Sergey Chechkin
/// Autor: Sergey Chechkin, schechkin@gmail.com 

#pragma once

#include "spatial_hash/SpatialHash2D.h"
#include "spatial_hash/HashContainers.h"

namespace libs::spatial_hash {

/// @brief 2D spatial hash table with vector container. 
/// @tparam DataType - 2D spase data type (float, double) 
/// @tparam RefType - associated data type 
template<typename DataType, typename RefType>
class SpatialHashTable2DVector : public SpatialHashTable2D<DataType, RefType, HashContainerVector<RefType>> {
public:
    using CellType = HashContainerVector<RefType>;
    using BaseClass = SpatialHashTable2D<DataType, RefType, HashContainerVector<RefType>>;
    using HashTableType = typename BaseClass::HashTableType;
public:
    SpatialHashTable2DVector() : BaseClass() {} 
    SpatialHashTable2DVector(DataType cell_size) : BaseClass(cell_size) {} 

    /// @brief Search all data references in specified square. Square parameters in discrete hash table space.
    /// @param center_cell - center cell
    /// @param half_size - half square size 
    /// @return - all data references in square  
    std::vector<RefType> SquareSearch(HashIndex2D center_cell, uint32_t half_size) const {
        std::vector<RefType> result;
        std::vector<const CellType*> cells = BaseClass::SquareSearch(center_cell, half_size);
        for(const CellType* cell : cells) {
            result.insert(result.end(), cell->begin(), cell->end());
        }
        return result;
    }

    /// @brief Search cells in square, that defined by two points
    /// @param left_top - left top corner 
    /// @param right_bottom  - right bottom corner
    /// @return - all data references in square 
    std::vector<RefType> SquareSearch(DataType left_top[2], DataType right_bottom[2]) const {
        std::vector<RefType> result;
        HashIndex2D lt_index = BaseClass::GetCellIndex(left_top);
        HashIndex2D rb_index = BaseClass::GetCellIndex(right_bottom);
        std::vector<const CellType*> cells = BaseClass::SquareSearch(lt_index, rb_index);
        for(const CellType* cell : cells) {
            result.insert(result.end(), cell->begin(), cell->end());
        }
        return result;
    }
};

}