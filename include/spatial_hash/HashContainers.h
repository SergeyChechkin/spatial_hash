/// BSD 3-Clause License
/// Copyright (c) 2023, Sergey Chechkin
/// Autor: Sergey Chechkin, schechkin@gmail.com 

#pragma once

#include <vector>

namespace libs::spatial_hash {

/// @brief Vector based container for spatial hash.
/// @tparam RefType - associated data type 
template<typename RefType>
class HashContainerVector : public std::vector<RefType> {
public:
    void Add(const RefType& v) {
        std::vector<RefType>::push_back(v);
    }
};

}