#include <iostream>
#include <memory>
#include <utility>
#include <vector>

#include "shared_types.h"
#include "student_a.h"

int main() {
    InputData input{
        {{2.0, -1.0, 3.0},
         {0.0,  4.0, 5.0},
         {1.0,  2.0, -2.0}},
        3
    };
    auto data = std::make_shared<const InputData>(std::move(input));

    std::cout << "Lab1: determinant of " << data->n << "x" << data->n << " matrix\n";

    // >>> ALGORITHMS CALL ZONE (Student A / Student B) <<<
    // TODO: call algorithms and print results
    // >>> END OF ZONE <<<

    return 0;
}