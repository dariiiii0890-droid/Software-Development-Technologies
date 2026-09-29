#include "student_a.h"

#include <cstddef>
#include <stdexcept>
#include <utility>
#include <vector>

namespace {

    using Matrix = std::vector<std::vector<double>>;

    // Мінор: матриця без рядка `row` і стовпця `col`
    Matrix buildMinor(const Matrix& m, std::size_t row, std::size_t col) {
        const std::size_t n = m.size();
        Matrix minor;
        minor.reserve(n - 1);
        for (std::size_t i = 0; i < n; ++i) {
            if (i == row) continue;
            std::vector<double> line;
            line.reserve(n - 1);
            for (std::size_t j = 0; j < n; ++j) {
                if (j != col) line.push_back(m[i][j]);
            }
            minor.push_back(std::move(line));
        }
        return minor;
    }

    // Рекурсивний розклад за елементами першого рядка.
    // Лічильник операцій: на кожен доданок 2 множення (знак * a[0][j] * мінор)
    // та 1 додавання, тобто 3 арифметичні операції.
    double laplaceDeterminant(const Matrix& m, long long& ops) {
        const std::size_t n = m.size();
        if (n == 1) {
            return m[0][0];
        }

        double det = 0.0;
        double sign = 1.0;
        for (std::size_t j = 0; j < n; ++j) {
            const double minorDet = laplaceDeterminant(buildMinor(m, 0, j), ops);
            det += sign * m[0][j] * minorDet;
            ops += 3;
            sign = -sign;
        }
        return det;
    }

    void validate(const std::shared_ptr<const InputData>& data) {
        if (!data) {
            throw std::invalid_argument("calculateA: data is null");
        }
        if (data->n <= 0 || data->matrix.size() != static_cast<std::size_t>(data->n)) {
            throw std::invalid_argument("calculateA: invalid matrix size");
        }
        for (const auto& row : data->matrix) {
            if (row.size() != static_cast<std::size_t>(data->n)) {
                throw std::invalid_argument("calculateA: matrix is not square");
            }
        }
    }

}  // namespace

std::unique_ptr<Result> calculateA(std::shared_ptr<const InputData> data) {
    validate(data);

    auto result = std::make_unique<Result>();
    result->determinant = laplaceDeterminant(data->matrix, result->operations_count);
    return result;
}