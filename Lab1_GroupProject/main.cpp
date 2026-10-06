#include <algorithm>
#include <chrono>
#include <cmath>
#include <exception>
#include <iomanip>
#include <iostream>
#include <memory>
#include <random>
#include <utility>
#include <vector>

#include "shared_types.h"
#include "student_a.h"

// Студент Б: реалізація знаходиться у student_b.cpp
std::unique_ptr<Result> calculateB(std::shared_ptr<const InputData> data);

namespace {

    using Clock = std::chrono::steady_clock;

    // Результат одного прогону алгоритму разом із виміряним часом
    struct Measurement {
        double determinant{ 0.0 };
        long long operations{ 0 };
        double microseconds{ 0.0 };
    };

    // Запускає алгоритм на спільних вхідних даних і вимірює час виконання
    template <typename Algorithm>
    Measurement measure(Algorithm algorithm, const std::shared_ptr<const InputData>& data) {
        const auto start = Clock::now();
        auto result = algorithm(data);  // std::unique_ptr<Result>
        const auto finish = Clock::now();

        auto [determinant, operations] = *result;  // structured bindings
        const double us = std::chrono::duration<double, std::micro>(finish - start).count();
        return { determinant, operations, us };
    }

    // Збіг визначників з урахуванням похибки обчислень з плаваючою комою
    bool nearlyEqual(double a, double b) {
        return std::fabs(a - b) <= 1e-6 * std::max({ 1.0, std::fabs(a), std::fabs(b) });
    }

    // Випадкова цілочисельна матриця n x n (значення від -5 до 5)
    std::shared_ptr<const InputData> makeRandomData(int n, std::mt19937& rng) {
        std::uniform_int_distribution<int> dist(-5, 5);
        InputData input{};
        input.n = n;
        input.matrix.assign(static_cast<std::size_t>(n), std::vector<double>(static_cast<std::size_t>(n)));
        for (auto& row : input.matrix) {
            for (auto& value : row) {
                value = static_cast<double>(dist(rng));
            }
        }
        return std::make_shared<const InputData>(std::move(input));
    }

    void printDemo() {
        // Один спільний об'єкт вхідних даних для обох алгоритмів
        InputData input{};
        input.matrix = {
            {2.0, -1.0, 3.0},
            {0.0, 4.0, 5.0},
            {1.0, 2.0, -2.0}
        };
        input.n = 3;
        auto data = std::make_shared<const InputData>(std::move(input));

        std::cout << "Lab1: determinant of " << data->n << "x" << data->n << " matrix\n";

        auto resultA = calculateA(data);
        auto resultB = calculateB(data);
        auto [detA, opsA] = *resultA;
        auto [detB, opsB] = *resultB;

        std::cout << "[Student A] Laplace expansion\n"
            << "  determinant = " << detA << "\n"
            << "  operations  = " << opsA << "\n";
        std::cout << "[Student B] Triangular method\n"
            << "  determinant = " << detB << "\n"
            << "  operations  = " << opsB << "\n";
        std::cout << "Determinants match: " << (nearlyEqual(detA, detB) ? "YES" : "NO")
            << " (difference = " << std::fabs(detA - detB) << ")\n";
    }

    void printComparisonTable(int minSize, int maxSize) {
        std::mt19937 rng(12345);  // фіксоване зерно: результати повторювані
        bool allMatch = true;

        std::cout << "\nComparison on random matrices (A = Laplace, B = Triangular)\n";
        std::cout << std::setw(3) << "n" << " |"
            << std::setw(16) << "det A" << " |"
            << std::setw(16) << "det B" << " |"
            << std::setw(6) << "match" << " |"
            << std::setw(11) << "ops A" << " |"
            << std::setw(7) << "ops B" << " |"
            << std::setw(13) << "time A, us" << " |"
            << std::setw(11) << "time B, us" << "\n";
        std::cout << std::string(3 + 2 + 16 + 2 + 16 + 2 + 6 + 2 + 11 + 2 + 7 + 2 + 13 + 2 + 11, '-') << "\n";

        std::cout << std::fixed;
        for (int n = minSize; n <= maxSize; ++n) {
            const auto data = makeRandomData(n, rng);  // спільні дані для обох алгоритмів
            const Measurement a = measure(calculateA, data);
            const Measurement b = measure(calculateB, data);
            const bool match = nearlyEqual(a.determinant, b.determinant);
            allMatch = allMatch && match;

            std::cout << std::setw(3) << n << " |"
                << std::setw(16) << std::setprecision(2) << a.determinant << " |"
                << std::setw(16) << std::setprecision(2) << b.determinant << " |"
                << std::setw(6) << (match ? "yes" : "NO") << " |"
                << std::setw(11) << a.operations << " |"
                << std::setw(7) << b.operations << " |"
                << std::setw(13) << std::setprecision(1) << a.microseconds << " |"
                << std::setw(11) << std::setprecision(1) << b.microseconds << "\n";
        }
        std::cout << std::defaultfloat;
        std::cout << "\nAll determinants match: " << (allMatch ? "YES" : "NO") << "\n";
        std::cout << "Laplace expansion grows like n!, the triangular method like n^3.\n";
    }

}  // namespace

int main() {
    // Найбільший розмір для таблиці. Метод Лапласа має складність O(n!),
    // тому значення вище 9-10 робить програму дуже повільною.
    constexpr int kMinSize = 3;
    constexpr int kMaxSize = 8;

    try {
        printDemo();
        printComparisonTable(kMinSize, kMaxSize);
    }
    catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << "\n";
        return 1;
    }
    return 0;
}