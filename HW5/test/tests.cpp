#include <gtest/gtest.h>
#include <string>

#include "Matrix.h"

using namespace std;

TEST (matrix2d, first_case){
    Matrix<int, -1> matrix;

    matrix[100][100] = 314;

    ASSERT_EQ(matrix[100][100], 314);
    ASSERT_EQ(matrix.size(), 1);
}

TEST (matrix2d, second_case){
    Matrix<int, -1> matrix;
    ASSERT_EQ(matrix.size(), 0);
}

TEST (matrix2d, third_case){
    Matrix<int, -1> matrix;
    auto a = matrix[0][0];
    auto b = matrix[99][99];
    ASSERT_EQ(a, -1);
    ASSERT_EQ(b, -1);
    ASSERT_EQ(matrix.size(), 0);
}

TEST (matrix3d, first_case){
    string str;
    Matrix<int, -13, 3> matrix;

    for (size_t i = 0; i < 3; i++){
        for (size_t j = 0; j < 3; j++){
            for (size_t k = 0; k < 3; k++){
                matrix[i][j][k] = k + j + i;
            }
        }
    }

    for (auto i : matrix){
        int x;
        int y;
        int z;
        int value;
        std::tie(x, y, z, value) = i;
        str += to_string(value) + '\n';
    }

    stringstream answer (str);

    stringstream expected {
            "0\n"
            "1\n"
            "2\n"
            "1\n"
            "2\n"
            "3\n"
            "2\n"
            "3\n"
            "4\n"
            "1\n"
            "2\n"
            "3\n"
            "2\n"
            "3\n"
            "4\n"
            "3\n"
            "4\n"
            "5\n"
            "2\n"
            "3\n"
            "4\n"
            "3\n"
            "4\n"
            "5\n"
            "4\n"
            "5\n"
            "6\n"
    };

    ASSERT_EQ(expected.str(), answer.str());
}

TEST (matrix3d, second_case){
    Matrix<int, -13, 3> matrix;
    for (size_t i = 0; i < 10; i++)
        ASSERT_EQ(matrix[i][i][i], -13);

    ASSERT_EQ(matrix.size(), 0);
}

TEST (matrix10d, first_case){
    Matrix<int, 0, 10> matrix;

    matrix[0][1][2][3][4][5][6][7][8][9] = 782;
    int a = 0;
    for (auto i : matrix)
    {
        int x1,x2,x3,x4,x5,x6,x7,x8,x9,x10;
        std::tie(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10, a) = i;
    }

    ASSERT_EQ(a, 782);
}