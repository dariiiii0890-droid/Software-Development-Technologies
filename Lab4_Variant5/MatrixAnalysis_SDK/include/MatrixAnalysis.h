#pragma once

#ifdef __cplusplus
extern "C" {
#endif

    // Статична збірка: MATRIX_STATIC; збірка самої DLL: MATRIX_EXPORTS; клієнт DLL: імпорт
#if defined(MATRIX_STATIC)
#define MATRIX_API
#elif defined(MATRIX_EXPORTS)
#define MATRIX_API __declspec(dllexport)
#else
#define MATRIX_API __declspec(dllimport)
#endif

#define MATRIX_MAX_SIZE 100

#define MATRIX_OK                 0
#define MATRIX_ERR_NULL_POINTER   1
#define MATRIX_ERR_INVALID_SIZE   2
#define MATRIX_ERR_INVALID_VALUE  3
#define MATRIX_ERR_SINGULAR       4

// Визначник квадратної матриці n x n
    MATRIX_API int determinant(const double* matrix, int n, double* result);

    // Обернена матриця; result - буфер на n*n елементів (виділяє викликач)
    MATRIX_API int inverse(const double* matrix, int n, double* result);

    // Ранг матриці rows x cols
    MATRIX_API int rank(const double* matrix, int rows, int cols, int* result);

    // *result = 1 якщо матриця вироджена, 0 якщо ні
    MATRIX_API int isSingular(const double* matrix, int n, int* result);

#ifdef __cplusplus
}
#endif