#pragma once

#if defined(EIGENLIB_STATIC)
#define EIGENLIB_API extern "C"
#elif defined(EIGENLIB_EXPORTS)
#define EIGENLIB_API extern "C" __declspec(dllexport)
#else
#define EIGENLIB_API extern "C" __declspec(dllimport)
#endif

EIGENLIB_API int powerMethod(const double* matrix, int n, int maxIterations,
                             double tolerance, double* eigenvalueOut,
                             double* eigenvectorOut);
EIGENLIB_API int eigenvalue(const double* matrix, const double* vector, int n,
                            double* eigenvalueOut);
EIGENLIB_API int eigenvector(const double* matrix, int n, int maxIterations,
                             double tolerance, double* eigenvectorOut);
EIGENLIB_API double residual(const double* matrix, const double* vector,
                             int n, double lambda);
