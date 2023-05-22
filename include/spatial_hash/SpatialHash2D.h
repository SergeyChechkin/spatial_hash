/// BSD 3-Clause License
/// Copyright (c) 2023, Sergey Chechkin
/// Autor: Sergey Chechkin, schechkin@gmail.com 

#pragma once

#include <unordered_map>
#include <vector>
#include <cmath>
#include <cstdint>

namespace libs::spatial_hash {

/// @brief 2D spatial hash index
struct HashIndex2D {
    HashIndex2D() :
        x_(0), y_(0) {}
    HashIndex2D(int32_t x, int32_t y) :
        x_(x), y_(y) {}

    int32_t x_;
    int32_t y_;

    friend bool operator == (const HashIndex2D& a, const HashIndex2D& b)
    {
        return !(a.x_ != b.x_ || a.y_ != b.y_);
    }

    friend HashIndex2D operator + (const HashIndex2D& a, const HashIndex2D& b)
    {
        return HashIndex2D(a.x_ + b.x_, a.y_ + b.y_);
    }
};

/// @brief 2D spatial hash function
struct SpatalHash2D {
    uint64_t operator() (const HashIndex2D& val) const
    {
        static int64_t max_size = std::pow(2, 31);
        return static_cast<uint64_t>(val.x_ + max_size) + 
            static_cast<uint64_t>((val.y_ + max_size) << 32u);
    }
};

/// @brief Base class for 2D spatal hash table. 
/// @tparam DataType - 2D spase data type (float, double)
/// @tparam RefType - point associated data type 
/// @tparam ContainerType - cell container type, must have Add(...) method 
template<typename DataType, typename RefType, typename ContainerType>
class SpatialHashTable2D {
protected:
    using HashTableType = std::unordered_map<HashIndex2D, ContainerType, SpatalHash2D>;

    DataType cell_size_;
    DataType inv_cell_size_;
    HashTableType table_;
public:
    /// @brief Default constructor 
    SpatialHashTable2D() : cell_size_(0), inv_cell_size_(0) {}

    /// @brief Constructor with cell size
    /// @param cell_size - size of cell 
    explicit SpatialHashTable2D(DataType cell_size) : cell_size_(cell_size), inv_cell_size_(1 / cell_size) {}
    
    /// @brief Sets cell size
    /// @param cell_size - size of cell
    void SetCellSize(DataType cell_size) {
        Clear();
        cell_size_ = cell_size;
        inv_cell_size_ = 1 / cell_size;
    }

    /// @brief Clear hash table
    void Clear() {
        table_.clear();
    }
    
    const HashTableType& GetTable() const {
        return table_;
    } 

    /// @brief Returns cell size.
    /// @return - cell size
    DataType GetCellSize() const {
        return cell_size_;
    } 

    /// @brief Returns inverse cell size.  
    /// @return - inverse cell size
    DataType GetInvVoxelSize() const {
        return inv_cell_size_;
    } 

    /// @brief Add value to hash table
    /// @param point - continuous 2D space point
    /// @param ref - associated data
    void Add(const DataType point[2], RefType ref) {
        HashIndex2D cell_index = GetCellIndex(point);
        table_[cell_index].Add(ref);
    }
    
    /// @brief Convert continuous 2D space point in discrete hash space index 
    /// @param point - continuous 2D space point
    /// @return hash table index
    HashIndex2D GetCellIndex(const DataType point[2]) const {
        HashIndex2D result;
        result.x_ = static_cast<int32_t>(std::floor(point[0] * inv_cell_size_));
        result.y_ = static_cast<int32_t>(std::floor(point[1] * inv_cell_size_));
        return result;
    }

protected:
    /// @brief Search data for specific cell. 
    /// @param cell_idx - cell index
    /// @return cell container 
    const ContainerType* GetCell(HashIndex2D cell_idx) const {
        auto itr = table_.find(cell_idx);
        if(table_.end() == itr) {
            return nullptr;
        }

        return &(itr->second);
    } 

    /// @brief Search all populated cells in (2 * half_size + 1) square of cells with "center" cell in center
    /// @param center - center cell
    /// @param half_size - half size of the search square
    /// @return Return all populated cells in square
    std::vector<const ContainerType*> SquareSearch(HashIndex2D center, int32_t half_size) const {
        std::vector<const ContainerType*> result;
        
        HashIndex2D grid_point;
        for(grid_point.x_ = center.x_ - half_size; grid_point.x_ <= center.x_ + half_size; ++grid_point.x_) {
            for(grid_point.y_ = center.y_ - half_size; grid_point.y_ <= center.y_ + half_size; ++grid_point.y_) {            
                auto itr = table_.find(grid_point);
                if(table_.end() == itr) {
                    continue;
                }
                result.push_back(&(itr->second));                
            }
        }
        
        return result;
    }

    /// @brief Search cells in square
    /// @param left_top - left top corner 
    /// @param right_bottom - right bottom corner
    /// @return Return all populated cells in square
    std::vector<const ContainerType*> SquareSearch(HashIndex2D left_top, HashIndex2D right_bottom) const {
        std::vector<const ContainerType*> result;
        
        if (right_bottom.x_ < left_top.x_) {
            std::swap(right_bottom.x_, left_top.x_);
        }        
        if (right_bottom.y_ < left_top.y_) {
            std::swap(right_bottom.y_, left_top.y_);
        }    

        HashIndex2D grid_point;
        for(grid_point.x_ = left_top.x_; grid_point.x_ <= right_bottom.x_; ++grid_point.x_) {
            for(grid_point.y_ = left_top.y_; grid_point.y_ <= right_bottom.y_; ++grid_point.y_) {            
                auto itr = table_.find(grid_point);
                if(table_.end() == itr) {
                    continue;
                }
                result.push_back(&(itr->second));                
            }
        }
        
        return result;
    }
};

}