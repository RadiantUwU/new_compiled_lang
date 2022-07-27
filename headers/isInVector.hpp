#pragma once
#include<vector>
#include<algorithm>
template <typename T>
inline bool isInVector(std::vector<T> l, T v) {
    return (std::find(l.begin(),l.end(),v)) != l.end();
}
template <typename T>
inline auto findInVector(std::vector<T> l, T v) {
    return (std::find(l.begin(),l.end(),v));
}