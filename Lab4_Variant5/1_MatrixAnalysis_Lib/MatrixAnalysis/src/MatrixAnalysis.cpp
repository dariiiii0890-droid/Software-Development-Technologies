#include "MatrixAnalysis.h"

#include <algorithm>
#include <cmath>
#include <utility>
#include <vector>

namespace {

    const double EPS = 1e-9;  // значення менше за EPS вважаються нулем

    bool hasInvalidValues(const double* data, int count) {
        for (int i = 0; i < count; ++i) {
            if (!std::isfinite(data[i])) return true;
        }
        return false;
    }

    int checkMatrix(const double* m, int rows, int cols) {
        if (m == nullptr) return MATRIX_ERR_NULL_POINTER;
        if (rows <= 0 || cols <= 0 || rows > MATRIX_MAX_SIZE || cols > MATRIX_MAX_SIZE)
            return MATRIX_ERR_INVALID_SIZE;
        if (hasInvalidValues(m, rows * cols)) return MATRIX_ERR_INVALID_VALUE;
        return MATRIX_OK;
    }

    void swapRows(std::vector<double>& a, int cols, int r1, int r2) {
        for (int c = 0; c < cols; ++c) std::swap(a[r1 * cols + c], a[r2 * cols + c]);
    }

    int rankOf(std::vector<double> a, int rows, int cols) {
        int rank = 0;
        for (int col = 0; col < cols && rank < rows; ++col) {
            int pivot = rank;
            for (int r = rank + 1; r < rows; ++r) {
                if (std::fabs(a[r * cols + col]) > std::fabs(a[pivot * cols + col])) pivot = r;
            }
            if (std::fabs(a[pivot * cols + col]) < EPS) continue;
            if (pivot != rank) swapRows(a, cols, pivot, rank);
            for (int r = rank + 1; r < rows; ++r) {
                double f = a[r * cols + col] / a[rank * cols + col];
                for (int c = col; c < cols; ++c) a[r * cols + c] -= f * a[rank * cols + c];
            }
            ++rank;
        }
        return rank;
    }

    double detOf(std::vector<double> a, int n) {
        double det = 1.0;
        for (int col = 0; col < n; ++col) {
            int pivot = col;
            for (int r = col + 1; r < n; ++r) {
                if (std::fabs(a[r * n + col]) > std::fabs(a[pivot * n + col])) pivot = r;
            }
            if (std::fabs(a[pivot * n + col]) < EPS) return 0.0;
            if (pivot != col) {
                swapRows(a, n, pivot, col);
                det = -det;
            }
            det *= a[col * n + col];
            for (int r = col + 1; r < n; ++r) {
                double f = a[r * n + col] / a[col * n + col];
                for (int c = col; c < n; ++c) a[r * n + c] -= f * a[col * n + c];
            }
        }
        return det;
    }

    bool invertOf(std::vector<double> a, int n, double* out) {
        std::vector<double> inv(n * n, 0.0);
        for (int i = 0; i < n; ++i) inv[i * n + i] = 1.0;

        for (int col = 0; col < n; ++col) {
            int pivot = col;
            for (int r = col + 1; r < n; ++r) {
                if (std::fabs(a[r * n + col]) > std::fabs(a[pivot * n + col])) pivot = r;
            }
            if (std::fabs(a[pivot * n + col]) < EPS) return false;  // вироджена
            if (pivot != col) {
                swapRows(a, n, pivot, col);
                swapRows(inv, n, pivot, col);
            }
            double p = a[col * n + col];
            for (int c = 0; c < n; ++c) {
                a[col * n + c] /= p;
                inv[col * n + c] /= p;
            }
            for (int r = 0; r < n; ++r) {
                if (r == col) continue;
                double f = a[r * n + col];
                if (f == 0.0) continue;
                for (int c = 0; c < n; ++c) {
                    a[r * n + c] -= f * a[col * n + c];
                    inv[r * n + c] -= f * inv[col * n + c];
                }
            }
        }
        std::copy(inv.begin(), inv.end(), out);
        return true;
    }

}  // namespace

int determinant(const double* matrix, int n, double* result) {
    if (result == nullptr) return MATRIX_ERR_NULL_POINTER;
    int err = checkMatrix(matrix, n, n);
    if (err != MATRIX_OK) return err;
    *result = detOf(std::vector<double>(matrix, matrix + n * n), n);
    return MATRIX_OK;
}

int inverse(const double* matrix, int n, double* result) {
    if (result == nullptr) return MATRIX_ERR_NULL_POINTER;
    int err = checkMatrix(matrix, n, n);
    if (err != MATRIX_OK) return err;
    if (!invertOf(std::vector<double>(matrix, matrix + n * n), n, result))
        return MATRIX_ERR_SINGULAR;
    return MATRIX_OK;
}

int rank(const double* matrix, int rows, int cols, int* result) {
    if (result == nullptr) return MATRIX_ERR_NULL_POINTER;
    int err = checkMatrix(matrix, rows, cols);
    if (err != MATRIX_OK) return err;
    *result = rankOf(std::vector<double>(matrix, matrix + rows * cols), rows, cols);
    return MATRIX_OK;
}

int isSingular(const double* matrix, int n, int* result) {
    if (result == nullptr) return MATRIX_ERR_NULL_POINTER;
    int err = checkMatrix(matrix, n, n);
    if (err != MATRIX_OK) return err;
    *result = (rankOf(std::vector<double>(matrix, matrix + n * n), n, n) < n) ? 1 : 0;
    return MATRIX_OK;
}