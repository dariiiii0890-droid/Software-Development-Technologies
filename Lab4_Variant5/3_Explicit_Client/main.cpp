#include <iostream>
#include <windows.h>

typedef int(*PowerMethodFunc)(const double*, int, int, double, double*, double*);
typedef double(*ResidualFunc)(const double*, const double*, int, double);

int main() {
    std::cout << "=== Explicit Client: Boundary Condition Tests ===\n";

    HMODULE hLib = LoadLibrary("EigenLib.dll");
    if (!hLib) {
        std::cerr << "Error: Could not load EigenLib.dll! Error code: " << GetLastError() << "\n";
        return 1;
    }

    PowerMethodFunc powerMethod = (PowerMethodFunc)GetProcAddress(hLib, "powerMethod");
    ResidualFunc residual = (ResidualFunc)GetProcAddress(hLib, "residual");

    if (!powerMethod || !residual) {
        std::cerr << "Error: Could not find functions in DLL!\n";
        FreeLibrary(hLib);
        return 1;
    }

    // Тест 1: Коректні дані (базовий випадок)
    std::cout << "\n[Test 1] Standard valid matrix:\n";
    double matrix[4] = { 4.0, 1.0, 2.0, 3.0 };
    double eigenvalue = 0.0;
    double eigenvector[2] = { 0.0, 0.0 };

    int res1 = powerMethod(matrix, 2, 100, 1e-6, &eigenvalue, eigenvector);
    std::cout << "Result code (expected 0): " << res1 << "\n";
    std::cout << "Eigenvalue: " << eigenvalue << "\n";

    // Тест 2: Граничний випадок — нульовий розмір матриці (dimension = 0)
    std::cout << "\n[Test 2] Invalid dimension (size = 0):\n";
    int res2 = powerMethod(matrix, 0, 100, 1e-6, &eigenvalue, eigenvector);
    std::cout << "Result code (expected error code): " << res2 << "\n";

    // Тест 3: Граничний випадок — нульовий вказівник на матрицю (nullptr)
    std::cout << "\n[Test 3] Nullptr matrix input:\n";
    int res3 = powerMethod(nullptr, 2, 100, 1e-6, &eigenvalue, eigenvector);
    std::cout << "Result code (expected error code): " << res3 << "\n";

    FreeLibrary(hLib);
    std::cout << "\nAll boundary tests completed successfully.\n";

    return 0;
}