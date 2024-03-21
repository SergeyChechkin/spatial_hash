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
template<typename KeyT, typename RefType, unsigned int limit>
class ContainerHeap : public std::priority_queue<std::pair<KeyT, RefType>, std::vector<std::pair<KeyT, RefType>>, std::greater<std::pair<KeyT, RefType>>>{
public:
    using BaseClass = std::priority_queue<std::pair<KeyT, RefType>, std::vector<std::pair<KeyT, RefType>>, std::greater<std::pair<KeyT, RefType>>>;
    void Add(KeyT key, const RefType& v) {
        if (BaseClass::size() < limit || BaseClass::top().first < key) {
            BaseClass::insert(key, v);
        } 

        while(BaseClass::size() > limit) {
            BaseClass::pop();
        }
    }
};

}