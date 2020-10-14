#include <iostream>
#include <cassert>
#include "Matrix.h"

int main(){
    using namespace std;

    Matrix<int, 0> matrix;
    for (size_t i = 0; i <= 9; i++){
            matrix[i][i] = i;
            matrix[i][9 - i] = 9 - i;
    }

    int g = 2;
    for (auto i : matrix){
        int x;
        int y;
        int z;
        std::tie(x, y, z) = i;
        if (x == g) {
            cout << endl;
            g++;
        }
        if (x > 0 && y > 0 && x < 9 && y < 9)
            std::cout << x << ' ' << y << ' ' << z << ' ';
    }
    cout << "size: " << matrix.size() << endl;

    ((matrix[100][100] = 314) = 0) = 217;
    cout << matrix[100][100] << endl;

}
