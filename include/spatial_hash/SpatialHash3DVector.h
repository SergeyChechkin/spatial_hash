/// BSD 3-Clause License
/// Copyright (c) 2023, Sergey Chechkin
/// Autor: Sergey Chechkin, schechkin@gmail.com 

#pragma once

#include "spatial_hash/SpatialHash3D.h"
#include "spatial_hash/Containers.h"

namespace libs::spatial_hash {

/// @brief 
/// @tparam DataType 
/// @tparam RefType 
template<typename DataType, typename RefType>
class SpatialHashTable3DVector : public SpatialHashTable3D<DataType, RefType, ContainerVector<RefType>> {
public:
    using CellType = ContainerVector<RefType>;
    using BaseClass = SpatialHashTable3D<DataType, RefType, ContainerVector<RefType>>;
    using HashTableType = typename BaseClass::HashTableType;
public:
    SpatialHashTable3DVector() : BaseClass() {} 
    SpatialHashTable3DVector(DataType cell_size) : BaseClass(cell_size) {} 

    /// @brief Returns data for specific voxel index
    /// @param index - voxel index 
    /// @return voxel references
    std::vector<RefType> GetVoxelData(HashIndex3D index) {        
        std::vector<RefType> result;
        auto cell = BaseClass::GetCell(index);
        if (cell) {
            result.insert(result.end(), cell->begin(), cell->end());
        }

        return result;
    } 

    /// @brief Search all data references in specified cube. Cube parameters in discrete hash table space.
    /// @param center - central voxel 
    /// @param half_size - half cube size
    /// @return all data references in cube
    std::vector<RefType> CubeSearch(HashIndex3D center, int32_t half_size) const {
        std::vector<RefType> result;
        std::vector<const CellType*> cells = BaseClass::CubeSearch(center, half_size);

        for(const CellType* cell : cells) {
            result.insert(result.end(), cell->begin(), cell->end());
        }
        return result;
    }

    /// @brief Search all data references in specified cube. Cube parameters in R3 space.
    /// @param center - central point 
    /// @param half_size - half cube size in R3
    /// @return all data references in cube
    std::vector<RefType> CubeSearch(const DataType center[3], float half_size) const {
        std::vector<RefType> result;
        HashIndex3D center_index = BaseClass::GetVoxelIndex(center);
        uint32_t half_size_i = half_size * BaseClass::GetInvVoxelSize();

        return CubeSearch(center_index, half_size_i);
    }
};

}