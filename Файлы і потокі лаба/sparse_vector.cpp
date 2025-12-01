#include "sparse_vector.h"

std::string trim(const std::string& str)
{
    size_t start = str.find_first_not_of(" \t\r\n");
    if (start == std::string::npos)
    {
        return "";
    }
    size_t end = str.find_last_not_of(" \t\r\n");
    return str.substr(start, end - start + 1);
}

void initVector(SparseVector& vec)
{
    vec.indices.clear();
    vec.values.clear();
}

void setCoordinate(SparseVector& vec, size_t index, int value)
{
    if (value == 0)
    {
        return;
    }
    for (size_t i = 0; i < vec.indices.size(); ++i)
    {
        if (vec.indices[i] == index)
        {
            vec.values[i] = value;
            return;
        }
    }
    vec.indices.push_back(index);
    vec.values.push_back(value);
}

int getCoordinate(const SparseVector& vec, size_t index)
{
    for (size_t i = 0; i < vec.indices.size(); ++i)
    {
        if (vec.indices[i] == index)
        {
            return vec.values[i];
        }
    }
    return 0;
}

size_t getNonZeroCount(const SparseVector& vec)
{
    return vec.indices.size();
}

void clearVector(SparseVector& vec)
{
    vec.indices.clear();
    vec.values.clear();
}

void printVector(const SparseVector& vec, std::ostream& os)
{
    for (size_t i = 0; i < vec.indices.size(); ++i)
    {
        os << std::format("[{}] = {}", vec.indices[i], vec.values[i]) << std::endl;
    }
    if (vec.indices.empty())
    {
        os << "Вектор пуст (все координаты нулевые)" << std::endl;
    }
}

long scalarProduct(const SparseVector& v1, const SparseVector& v2)
{
    long result = 0;
    if (getNonZeroCount(v1) <= getNonZeroCount(v2))
    {
        for (size_t i = 0; i < getNonZeroCount(v1); ++i)
        {
            size_t index = v1.indices[i];
            result += static_cast<long>(v1.values[i]) * getCoordinate(v2, index);
        }
    }
    else
    {
        for (size_t i = 0; i < getNonZeroCount(v2); ++i)
        {
            size_t index = v2.indices[i];
            result += static_cast<long>(v2.values[i]) * getCoordinate(v1, index);
        }
    }
    return result;
}