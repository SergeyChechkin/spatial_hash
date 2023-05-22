/// BSD 3-Clause License
/// Copyright (c) 2023, Sergey Chechkin
/// Autor: Sergey Chechkin, schechkin@gmail.com 

#pragma once

#include <unordered_map>
#include <cmath>
#include <cstdint>

namespace libs::spatial_hash {

/// @brief 3D hash table index type 
/// supported cells dimention value range [-1048576 .. 1048575]
struct HashIndex3D {
    HashIndex3D() :
        x_(0), y_(0), z_(0) {}
    HashIndex3D(int32_t x, int32_t y, int32_t z) :
        x_(x), y_(y), z_(z) {}

    int32_t x_;
    int32_t y_;
    int32_t z_;

    friend bool operator == (const HashIndex3D& a, const HashIndex3D& b)
    {
        return !(a.x_ != b.x_ || a.y_ != b.y_ || a.z_ != b.z_);
    }

    friend HashIndex3D operator + (const HashIndex3D& lhs, const HashIndex3D& rhs)
    {
        return HashIndex3D(lhs.x_ + rhs.x_, lhs.y_ + rhs.y_, lhs.z_ + rhs.z_);
    }
};

struct SpatalHash3D {
    uint64_t operator() (const HashIndex3D& val) const
    {
        static int64_t max_size = std::pow(2, 20);
        return static_cast<uint64_t>(val.x_ + max_size) + 
            static_cast<uint64_t>((val.y_ + max_size) << 21u) + 
            static_cast<uint64_t>((val.z_ + max_size) << 42u);
    }
};

/// DataType - float, double
/// RefType - associated data
/// ContainerType - voxel container type, must have Add(...) method

/// @brief  Base class for 3D spatal hash table.
/// @tparam DataType - 3D spase data type (float, double)
/// @tparam RefType - point associated data type 
/// @tparam ContainerType - voxel container type, must have Add(...) method 
template<typename DataType, typename RefType, typename ContainerType>
class SpatialHashTable3D {
protected:
    using HashTableType = std::unordered_map<HashIndex3D, ContainerType, SpatalHash3D>;

    DataType voxel_size_;
    DataType inv_voxel_size_;
    HashTableType table_;
public:

    /// @brief Default constructor. 
    SpatialHashTable3D() : voxel_size_(0), inv_voxel_size_(0) {}

    /// @brief Constructor with voxel size.
    /// @param voxel_size - voxel size
    explicit SpatialHashTable3D(DataType voxel_size) : voxel_size_(voxel_size), inv_voxel_size_(1 / voxel_size) {}

    /// @brief Sets voxel size.
    /// @param voxel_size - voxel size
    void SetVoxelSize(DataType voxel_size) {
        Clear();
        voxel_size_ = voxel_size;
        inv_voxel_size_ = 1 / voxel_size;
    }

    /// @brief Clear the hash table. 
    void Clear() {
        table_.clear();
    }

    /// @brief Returns voxel size. 
    /// @return voxel size
    DataType GetVoxelSize() const {
        return voxel_size_;
    } 

    /// @brief Returns inverse voxel size.
    /// @return - inverse voxel size
    DataType GetInvVoxelSize() const {
        return inv_voxel_size_;
    } 

    /// @brief Returns hash table
    /// @return hash table reference 
    const HashTableType& GetTable() const {
        return table_;
    } 
    
    /// @brief Add value to hash table
    /// @param point - continuous 3D space point
    /// @param ref - associated data 
    void Add(const DataType point[3], RefType ref) {
        HashIndex3D voxel_index = GetVoxelIndex(point);
        table_[voxel_index].Add(ref);
    }

    /// @brief Convert continuous 3D space point in discrete hash space index 
    /// @param point - continuous 3D space point
    /// @return hash table index
    HashIndex3D GetVoxelIndex(const DataType point[3]) const {
        HashIndex3D result;
        result.x_ = static_cast<int32_t>(std::floor(point[0] * inv_voxel_size_));
        result.y_ = static_cast<int32_t>(std::floor(point[1] * inv_voxel_size_));
        result.z_ = static_cast<int32_t>(std::floor(point[2] * inv_voxel_size_));
        return result;
    }

protected:
    /// @brief Returns voxel container pointer. 
    /// @param index - voxel index 
    /// @return voxel container pointer  
    const ContainerType* GetVoxel(HashIndex3D index) const {
        auto itr = table_.find(index);
        if(table_.end() == itr) {
            return nullptr;
        }

        return &(itr->second);
    } 

    /// @brief Search all populated cells in (2 * half_size + 1) cube of voxels with "center" voxel in center
    /// @param center - center voxel
    /// @param half_size - half size of the search cube
    /// @return Return all populated cells in the cube
    std::vector<const ContainerType*> CubeSearch(HashIndex3D center, int32_t half_size) const {
        std::vector<const ContainerType*> result;
        
        HashIndex3D grid_point;
        for(grid_point.x_ = center.x_ - half_size; grid_point.x_ <= center.x_ + half_size; ++grid_point.x_) {
            for(grid_point.y_ = center.y_ - half_size; grid_point.y_ <= center.y_ + half_size; ++grid_point.y_) {
                for(grid_point.z_ = center.z_ - half_size; grid_point.z_ <= center.z_ + half_size; ++grid_point.z_) {            
                    auto itr = table_.find(grid_point);
                    if(table_.end() == itr) {
                        continue;
                    }
                    result.push_back(&(itr->second));                
                }
            }
        }

        return result;
    }
};

}