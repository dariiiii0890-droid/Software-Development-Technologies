#pragma once

#include <vector>

// Вхідні дані: квадратна матриця n x n
struct InputData {
    std::vector<std::vector<double>> matrix;
    int n;
};

// Результат обчислення
struct Result {
    double determinant{ 0.0 };
    long long operations_count{ 0 };
};