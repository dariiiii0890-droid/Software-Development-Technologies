#include <iostream>
#include "MatrixAnalysis.h"

int main() {
    // det = 4*6 - 7*2 = 10; обернена = [[0.6, -0.7], [-0.2, 0.4]]
    double a[4] = { 4, 7,
                   2, 6 };
    double det = 0;
    int code = determinant(a, 2, &det);
    std::cout << "determinant: code=" << code << " value=" << det << " (expected 10)\n";

    double inv[4] = {};
    code = inverse(a, 2, inv);
    std::cout << "inverse: code=" << code << "\n  "
        << inv[0] << " " << inv[1] << "\n  " << inv[2] << " " << inv[3] << "\n";

    // вироджена матриця 3x3: det = 0, rank = 2
    double s[9] = { 1, 2, 3,
                   4, 5, 6,
                   7, 8, 9 };
    int r = -1, sing = -1;
    rank(s, 3, 3, &r);
    isSingular(s, 3, &sing);
    determinant(s, 3, &det);
    std::cout << "singular 3x3: det=" << det << " rank=" << r
        << " isSingular=" << sing << " (expected 0, 2, 1)\n";

    // inverse вирожденої має повернути код 4
    double inv3[9] = {};
    std::cout << "inverse(singular) code=" << inverse(s, 3, inv3) << " (expected 4)\n";

    // прямокутна матриця 2x3, rank = 1
    double rect[6] = { 1, 2, 3,
                      2, 4, 6 };
    rank(rect, 2, 3, &r);
    std::cout << "rank 2x3 = " << r << " (expected 1)\n";

    // некоректні дані
    std::cout << "nullptr -> code " << determinant(nullptr, 2, &det) << " (expected 1)\n";
    std::cout << "n = 0   -> code " << determinant(a, 0, &det) << " (expected 2)\n";
    return 0;
}