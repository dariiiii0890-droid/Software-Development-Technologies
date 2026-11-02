
#pragma once

#include <cstddef>
#include <functional>

enum class OptimizationMode {
    Minimum,
    Maximum
};

struct OptimizationResult {
    double x;
    double value;
    std::size_t iterations;
};

class OptimizationTask {
private:
    double left_;
    double right_;
    double tolerance_;
    std::size_t maxIterations_;

    OptimizationResult result_;
    bool hasResult_;

public:
    OptimizationTask(
        double left,
        double right,
        double tolerance,
        std::size_t maxIterations = 1000
    );

    OptimizationResult optimize(
        const std::function<double(double)>& function,
        double left,
        double right,
        double tolerance,
        OptimizationMode mode = OptimizationMode::Minimum
    );

    double getLeft() const;
    double getRight() const;
    double getTolerance() const;
    bool hasResult() const;
    OptimizationResult getResult() const;
};
