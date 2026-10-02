#include <iostream>
#include <memory>
#include <utility>
#include <vector>

#include "shared_types.h"
#include "student_a.h"

std::unique_ptr<Result> calculateB(
    std::shared_ptr<const InputData> data);

int main() {
    InputData input{};
    input.matrix = {
        {2.0, -1.0, 3.0},
        {0.0, 4.0, 5.0},
        {1.0, 2.0, -2.0}
    };
    input.n = 3;

    auto data = std::make_shared<const InputData>(std::move(input));

    std::cout << "Lab1: determinant of "
        << data->n << "x" << data->n << " matrix\n";

    // >>> ALGORITHMS CALL ZONE (Student A / Student B) <<<

    auto resultB = calculateB(data);
    auto [valueB, operationsB] = *resultB;

    std::cout << "Student B - Triangular method\n";
    std::cout << "Determinant: " << valueB << '\n';
    std::cout << "Operations: " << operationsB << '\n';

    // >>> END OF ZONE <<<

    return 0;
}