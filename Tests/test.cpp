#include "pch.h"
#include "../Файлы і потокі лаба/sparse_vector.cpp"
#include "../Файлы і потокі лаба/file_operations.cpp"
#include <sstream>

TEST(SparseVectorTest, TrimFunction)
{
    EXPECT_EQ(trim(""), "");
    EXPECT_EQ(trim("  hello  "), "hello");
    EXPECT_EQ(trim("\ttext\n"), "text");
    EXPECT_EQ(trim("   "), "");
    EXPECT_EQ(trim("no spaces"), "no spaces");
}

TEST(SparseVectorTest, SetAndGetCoordinate)
{
    SparseVector vec;
    initVector(vec);

    setCoordinate(vec, 0, 5);
    setCoordinate(vec, 3, 10);
    setCoordinate(vec, 7, 15);
    setCoordinate(vec, 3, 20);
    setCoordinate(vec, 5, 0);

    EXPECT_EQ(getCoordinate(vec, 0), 5);
    EXPECT_EQ(getCoordinate(vec, 3), 20);
    EXPECT_EQ(getCoordinate(vec, 7), 15);
    EXPECT_EQ(getCoordinate(vec, 5), 0);
    EXPECT_EQ(getCoordinate(vec, 100), 0);

    EXPECT_EQ(getNonZeroCount(vec), 3);
}

TEST(SparseVectorTest, ClearVector)
{
    SparseVector vec;
    initVector(vec);

    setCoordinate(vec, 0, 5);
    setCoordinate(vec, 1, 10);
    EXPECT_EQ(getNonZeroCount(vec), 2);

    clearVector(vec);
    EXPECT_EQ(getNonZeroCount(vec), 0);
    EXPECT_EQ(getCoordinate(vec, 0), 0);
}

TEST(SparseVectorTest, ScalarProductBasic)
{
    SparseVector v1;
    SparseVector v2;
    initVector(v1);
    initVector(v2);

    setCoordinate(v1, 0, 2);
    setCoordinate(v1, 2, 3);
    setCoordinate(v1, 4, 5);

    setCoordinate(v2, 0, 4);
    setCoordinate(v2, 2, 6);
    setCoordinate(v2, 3, 8);

    long result = scalarProduct(v1, v2);
    EXPECT_EQ(result, 2 * 4 + 3 * 6 + 5 * 0);
}

TEST(SparseVectorTest, ScalarProductEmpty)
{
    SparseVector v1;
    SparseVector v2;
    initVector(v1);
    initVector(v2);

    setCoordinate(v1, 0, 1);
    setCoordinate(v1, 1, 2);

    long result = scalarProduct(v1, v2);
    EXPECT_EQ(result, 0);

    result = scalarProduct(v2, v1);
    EXPECT_EQ(result, 0);
}

TEST(SparseVectorTest, ScalarProductNoOverlap)
{
    SparseVector v1;
    SparseVector v2;
    initVector(v1);
    initVector(v2);

    setCoordinate(v1, 0, 1);
    setCoordinate(v1, 2, 3);

    setCoordinate(v2, 1, 2);
    setCoordinate(v2, 3, 4);

    long result = scalarProduct(v1, v2);
    EXPECT_EQ(result, 0);
}

TEST(SparseVectorTest, PrintVector)
{
    SparseVector vec;
    initVector(vec);

    setCoordinate(vec, 1, 10);
    setCoordinate(vec, 3, 30);
    setCoordinate(vec, 5, 50);

    std::stringstream ss;
    printVector(vec, ss);
    std::string output = ss.str();

    EXPECT_TRUE(output.find("[1] = 10") != std::string::npos);
    EXPECT_TRUE(output.find("[3] = 30") != std::string::npos);
    EXPECT_TRUE(output.find("[5] = 50") != std::string::npos);
}

TEST(SparseVectorTest, PrintEmptyVector)
{
    SparseVector vec;
    initVector(vec);

    std::stringstream ss;
    printVector(vec, ss);
    std::string output = ss.str();

    EXPECT_TRUE(output.find("Вектор пуст") != std::string::npos);
}

TEST(SparseVectorTest, ReadVectorFromStream)
{
    std::stringstream ss;
    ss << "1 10\n";
    ss << "3 30\n";
    ss << "5 50\n";
    ss << "end\n";

    SparseVector vec;
    initVector(vec);
    bool result = readVectorFromStream(ss, vec);

    EXPECT_TRUE(result);
    EXPECT_EQ(getNonZeroCount(vec), 3);
    EXPECT_EQ(getCoordinate(vec, 1), 10);
    EXPECT_EQ(getCoordinate(vec, 3), 30);
    EXPECT_EQ(getCoordinate(vec, 5), 50);
}

TEST(SparseVectorTest, ReadVectorFromStreamInvalidFormat)
{
    std::stringstream ss;
    ss << "1 10 20\n";
    ss << "end\n";

    SparseVector vec;
    initVector(vec);
    bool result = readVectorFromStream(ss, vec);

    EXPECT_FALSE(result);
}

TEST(SparseVectorTest, ReadVectorsFromFile)
{
    std::ofstream testFile("test_input.txt");
    testFile << "1 10\n";
    testFile << "3 30\n";
    testFile << "---\n";
    testFile << "2 20\n";
    testFile << "4 40\n";
    testFile.close();

    SparseVector v1;
    SparseVector v2;
    initVector(v1);
    initVector(v2);

    bool result = readVectorsFromFile("test_input.txt", v1, v2);

    EXPECT_TRUE(result);
    EXPECT_EQ(getNonZeroCount(v1), 2);
    EXPECT_EQ(getNonZeroCount(v2), 2);
    EXPECT_EQ(getCoordinate(v1, 1), 10);
    EXPECT_EQ(getCoordinate(v1, 3), 30);
    EXPECT_EQ(getCoordinate(v2, 2), 20);
    EXPECT_EQ(getCoordinate(v2, 4), 40);

    std::remove("test_input.txt");
}

TEST(SparseVectorTest, ReadVectorsFromFileNoSeparator)
{
    std::ofstream testFile("test_input2.txt");
    testFile << "1 10\n";
    testFile << "3 30\n";
    testFile.close();

    SparseVector v1;
    SparseVector v2;
    initVector(v1);
    initVector(v2);

    bool result = readVectorsFromFile("test_input2.txt", v1, v2);

    EXPECT_TRUE(result);
    EXPECT_EQ(getNonZeroCount(v1), 2);
    EXPECT_EQ(getNonZeroCount(v2), 0);

    std::remove("test_input2.txt");
}
