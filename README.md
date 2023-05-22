# Spatial Hash

This Spatial Hash library is a set of template classes that provides **O(1)** spatial search capability in 2D / 3D Euclidean space. 

## Radius search

The library provides only square / cube search capabilities in discrete grid space. It doesn’t store 2D / 3D points. 

In order to perform radius search:
- Define square / cube that include circle / sphere of interest.
- Perform square / cube search to get points references.
- Perform radius search for square / cube subset.
```c++ 
std::vector<Eigen::Vector3f> point_cloud;
...
// populate hash table
SpatialHashTable3DVector<float, size_t> hash_table(0.1f); 
for(size_t i = 0; i < point_cloud.size(); ++i) {
    hash_table.Add(point_cloud[i].data(), i);
}
...
// peform square search 
float radius = 1.5;
Eigen::Vector3f center(0, 0, 0);
auto cube_idxs = hash_table.CubeSearch(center.data(), radius);
...    
// peform radius search
std::vector<Eigen::Vector3f> radius_search_result;
float radius_sqr = radius * radius;
for(auto idx : cube_idxs) {
    if ((point_cloud[idx]-center).squaredNorm() < radius_sqr) {
        radius_search_result.push_back(point_cloud[idx]);
    }
}
```
