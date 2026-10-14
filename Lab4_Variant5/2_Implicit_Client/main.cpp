#include <iostream>
#include "EigenLib.h"

int main() {
    std::cout << "Implicit Client started...\n";

    double matrix[4] = { 4.0, 1.0, 2.0, 3.0 };
    double eigenvalue = 0.0;
    double eigenvector[2] = { 0.0, 0.0 };

    int result = powerMethod(matrix, 2, 100, 1e-6, &eigenvalue, eigenvector);

    std::cout << "Result code: " << result << "\n";
    std::cout << "Eigenvalue: " << eigenvalue << "\n";
    std::cout << "Eigenvector: [" << eigenvector[0] << ", " << eigenvector[1] << "]\n";

    double res = residual(matrix, eigenvector, 2, eigenvalue);
    std::cout << "Residual: " << res << "\n";

    return 0;
}