#ifndef UTIL_H
#define UTIL_H

#include <vector>
#include <ostream>
#include <iostream>
#include "glm/glm.hpp"

#include <eigen3/Eigen/Dense>

template <typename T>
std::vector<double> linspace(T start_in, T end_in, int num_in, bool include_end);

template <typename T>
std::ostream &operator<<(std::ostream &os, const std::vector<T> &v);

std::ostream &operator<<(std::ostream &os, const glm::mat4 &m);

template <int N, typename T>
std::ostream &operator<<(std::ostream &os, const glm::vec<N, T> &v)
{
    for (size_t i = 0; i < v.length(); i++)
    {
        os << v[i] << ' ';
    }
    return os;
}

template <typename T>
glm::vec3 vec3_eigen_to_glm(Eigen::Vector3<T> v)
{
    return glm::vec3(v[0], v[1], v[2]);
}

inline int posMod(int i, int n)
{
    return (i % n + n) % n;
}

template <typename T>
int sgn(T val)
{
    return (T(0) < val) - (val < T(0));
}

#endif