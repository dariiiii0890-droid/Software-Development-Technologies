#include "shared_types.h"

#include <cmath>
#include <memory>
#include <vector>

std::unique_ptr<Result> calculateB(
    std::shared_ptr<const InputData> data)
{
    std::vector<std::vector<double>> matrix = data->matrix;

    const std::size_t n = matrix.size();

    long long operations = 0;
    int swaps = 0;

    for (std::size_t i = 0; i < n; ++i)
    {
        std::size_t pivot = i;

        for (std::size_t row = i + 1; row < n; ++row)
        {
            ++operations;

            if (std::abs(matrix[row][i]) > std::abs(matrix[pivot][i]))
            {
                pivot = row;
            }
        }

        if (std::abs(matrix[pivot][i]) < 1e-12)
        {
            auto result = std::make_unique<Result>();
            result->determinant = 0.0;
            result->operations_count = operations;
            return result;
        }

        if (pivot != i)
        {
            std::swap(matrix[i], matrix[pivot]);
            ++swaps;
        }

        for (std::size_t row = i + 1; row < n; ++row)
        {
            double factor = matrix[row][i] / matrix[i][i];
            ++operations;

            for (std::size_t col = i; col < n; ++col)
            {
                matrix[row][col] -= factor * matrix[i][col];
                ++operations;
            }
        }
    }

    double determinant = 1.0;

    for (std::size_t i = 0; i < n; ++i)
    {
        determinant *= matrix[i][i];
        ++operations;
    }

    if (swaps % 2 != 0)
    {
        determinant = -determinant;
    }

    auto result = std::make_unique<Result>();
    result->determinant = determinant;
    result->operations_count = operations;

    return result;
}