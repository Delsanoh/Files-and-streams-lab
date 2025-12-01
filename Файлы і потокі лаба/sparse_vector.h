#ifndef SPARSE_VECTOR_H
#define SPARSE_VECTOR_H

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <cctype>
#include <format>

struct SparseVector
{
    std::vector<size_t> indices;
    std::vector<int> values;
};

std::string trim(const std::string& str);
void initVector(SparseVector& vec);
void setCoordinate(SparseVector& vec, size_t index, int value);
int getCoordinate(const SparseVector& vec, size_t index);
size_t getNonZeroCount(const SparseVector& vec);
void clearVector(SparseVector& vec);
void printVector(const SparseVector& vec, std::ostream& os);
long scalarProduct(const SparseVector& v1, const SparseVector& v2);
bool readVectorsFromFile(const std::string& filename, SparseVector& v1, SparseVector& v2);
bool readVectorFromStream(std::istream& is, SparseVector& vector);
void readVectorFromConsole(SparseVector& vector, const std::string& prompt);

#endif