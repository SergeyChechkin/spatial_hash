/// BSD 3-Clause License
/// Copyright (c) 2023, Sergey Chechkin
/// Autor: Sergey Chechkin, schechkin@gmail.com 

#pragma once

#include <vector>
#include <map>
#include <queue>

namespace libs::spatial_hash {

/// @brief Vector based container for spatial hash.
/// @tparam RefType - associated data type 
template<typename RefType>
class ContainerVector : public std::vector<RefType> {
public:
    void Add(const RefType& v) {
        std::vector<RefType>::push_back(v);
    }
};

/// @brief Priority queue based contaner with size limit.
/// @tparam RefType - associated data type 
template<typename KeyT, typename RefType>
class ContainerHeap : public std::map<KeyT, RefType> {
private:
    using BaseClass = std::map<KeyT, RefType>;
public:
    void Add(KeyT key, const RefType& v, size_t limit) {
        if (BaseClass::size() < limit || BaseClass::begin()->first < key) {
            BaseClass::emplace(key, v);
        } 

        while(BaseClass::size() > limit) {
            BaseClass::erase(BaseClass::begin());
        }
    }
};

}