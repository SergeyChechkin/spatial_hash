/// BSD 3-Clause License
/// Copyright (c) 2023, Sergey Chechkin
/// Autor: Sergey Chechkin, schechkin@gmail.com 

#pragma once

#include "spatial_hash/SpatialHash2D.h"
#include "spatial_hash/Containers.h"

namespace libs::spatial_hash {

/// @brief 2D spatial hash table with vector container. 
/// @tparam DataType - 2D spase data type (float, double) 
/// @tparam RefType - associated data type 
template<typename DataType, typename RefType>
class SpatialHashTable2DVector : public SpatialHashTable2D<DataType, RefType, ContainerVector<RefType>> {
public:
    using CellType = ContainerVector<RefType>;
    using BaseClass = SpatialHashTable2D<DataType, RefType, ContainerVector<RefType>>;
    using HashTableType = typename BaseClass::HashTableType;
public:
    SpatialHashTable2DVector() : BaseClass() {} 
    SpatialHashTable2DVector(DataType cell_size) : BaseClass(cell_size) {} 

    /// @brief Search all data references in specified square. Square parameters in discrete hash table space.
    /// @param center_cell - center cell
    /// @param half_size - half square size 
    /// @return all data references in the square  
    std::vector<RefType> SquareSearch(HashIndex2D center_cell, int32_t half_size) const {
        std::vector<RefType> result;
        std::vector<const CellType*> cells = BaseClass::SquareSearch(center_cell, half_size);
        for(const CellType* cell : cells) {
            result.insert(result.end(), cell->begin(), cell->end());
        }
        return result;
    }

    /// @brief Search all data references in specified square. Square parameters in discrete hash table space.
    /// @param left_top - left top corner 
    /// @param right_bottom - right bottom corner 
    /// @return all data references in the square
    std::vector<RefType> SquareSearch(HashIndex2D left_top, HashIndex2D right_bottom) const {
        std::vector<RefType> result;
        std::vector<const CellType*> cells = BaseClass::SquareSearch(left_top, right_bottom);
        for(const CellType* cell : cells) {
            result.insert(result.end(), cell->begin(), cell->end());
        }
        return result;
    }

    /// @brief Search cells in square, that defined by two points
    /// @param left_top - left top corner 
    /// @param right_bottom - right bottom corner
    /// @return all data references in the square 
    std::vector<RefType> SquareSearch(const DataType left_top[2], const DataType right_bottom[2]) const {
        HashIndex2D lt_index = BaseClass::GetCellIndex(left_top);
        HashIndex2D rb_index = BaseClass::GetCellIndex(right_bottom);
        return SquareSearch(lt_index, rb_index);
    }

    /// @brief Search cells in square, that defined in R2 space 
    /// @param center - square center
    /// @param half_size - half square size
    /// @return all data references in the square
    std::vector<RefType> SquareSearch(const DataType center[2], DataType half_size) const {
        HashIndex2D center_index = BaseClass::GetCellIndex(center);
        uint32_t half_size_i = half_size * BaseClass::GetInvVoxelSize();
        return SquareSearch(center_index, half_size_i);
    } 

    /// @brief Retrieve data from the cell 
    /// @param cell_index - cell index
    /// @return data references in the cell 
    std::vector<RefType> GetCellData(HashIndex2D cell_index) {        
        std::vector<RefType> result;
        auto cell = BaseClass::GetCell(cell_index);
        if (cell) {
            result.insert(result.end(), cell->begin(), cell->end());
        }
        return result;
    }
};

}