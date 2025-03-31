#include "util.h"

template <typename T>
extern std::vector<double> linspace(T start_in, T end_in, int num_in, bool include_end)
{

    std::vector<double> linspaced(num_in);

    double start = static_cast<double>(start_in);
    double end = static_cast<double>(end_in);
    double num = static_cast<double>(num_in);

    if (num == 0)
    {
        return linspaced;
    }
    if (num == 1)
    {
        linspaced[0] = start;
        return linspaced;
    }

    if (!include_end)
        num += 1;

    double delta = (end - start) / (num - 1);

    for (int i = 0; i < num - 1; ++i)
    {
        linspaced[i] = start + delta * i;
    }

    if (include_end)
        linspaced[num_in - 1] = end;

    return linspaced;
}

template <typename T>
std::ostream &operator<<(std::ostream &os, const std::vector<T> &v)
{
    for (size_t i = 0; i < v.size(); i++)
    {
        os << v[i] << ' ';
    }
    return os;
}

// template <typename T>
// glm::vec3 vec3_eigen_to_glm(Eigen::Vector3<T> v)
// {
//     return glm::vec3(v[0], v[1], v[2]);
// }

std::ostream &operator<<(std::ostream &os, const glm::mat4 &m)
{
    for (size_t i = 0; i < 4; i++)
    {
        for (size_t j = 0; j < 4; j++)
        {
            os << m[j][i] << ' ';
        }
        os << std::endl;
    }
    return os;
}

template extern std::vector<double> linspace<double>(double start_in, double end_in, int num_in, bool include_end);