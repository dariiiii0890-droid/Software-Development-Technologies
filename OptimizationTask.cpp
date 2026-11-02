
#include "OptimizationTask.h"
#include "OptimizationExceptions.h"
#include "ScopeLogger.h"

#include <cmath>
#include <iostream>
#include <stdexcept>

namespace {

    constexpr double GOLDEN_RATIO = 0.6180339887498948482;

    double evaluateFunction(
        const std::function<double(double)>& function,
        double x
    ) {
        ScopeLogger logger("evaluateFunction");

        double value = function(x);

        if (!std::isfinite(value)) {
            throw InvalidFunctionValueError(
                "Function returned NaN or infinity"
            );
        }

        return value;
    }

    bool isBetter(
        double first,
        double second,
        OptimizationMode mode
    ) {
        if (mode == OptimizationMode::Minimum) {
            return first < second;
        }

        return first > second;
    }
    OptimizationResult search(
        const std::function<double(double)>& function,
        double left,
        double right,
        double tolerance,
        std::size_t maxIterations,
        OptimizationMode mode
    ) {
        ScopeLogger logger("search");

        try {
            double x1 = right - GOLDEN_RATIO * (right - left);
            double x2 = left + GOLDEN_RATIO * (right - left);

            double f1 = evaluateFunction(function, x1);
            double f2 = evaluateFunction(function, x2);

            std::size_t iterations = 0;

            while (right - left > tolerance) {
                if (iterations >= maxIterations) {
                    throw IterationLimitError(
                        "Maximum number of iterations exceeded"
                    );
                }

                double midpoint = left + (right - left) / 2.0;

                if (midpoint == left || midpoint == right) {
                    throw NoConvergenceError(
                        "Search interval cannot be reduced further"
                    );
                }

                if (isBetter(f1, f2, mode)) {
                    right = x2;
                    x2 = x1;
                    f2 = f1;

                    x1 = right - GOLDEN_RATIO * (right - left);
                    f1 = evaluateFunction(function, x1);
                }
                else {
                    left = x1;
                    x1 = x2;
                    f1 = f2;

                    x2 = left + GOLDEN_RATIO * (right - left);
                    f2 = evaluateFunction(function, x2);
                }

                ++iterations;
            }

            double resultX = left + (right - left) / 2.0;
            double resultValue = evaluateFunction(function, resultX);

            return { resultX, resultValue, iterations };
        }
        catch (const OptimizationError& error) {
            std::cerr << "[ERROR] Search failed: "
                << error.what() << '\n';
            throw;
        }
    }

}

OptimizationTask::OptimizationTask(
    double left,
    double right,
    double tolerance,
    std::size_t maxIterations
)
    : left_(left),
    right_(right),
    tolerance_(tolerance),
    maxIterations_(maxIterations),
    result_{ 0.0, 0.0, 0 },
    hasResult_(false) {

    if (!std::isfinite(left_) ||
        !std::isfinite(right_) ||
        left_ >= right_) {
        throw InvalidIntervalError(
            "Invalid search interval"
        );
    }

    if (!std::isfinite(tolerance_) || tolerance_ <= 0.0) {
        throw InvalidToleranceError(
            "Tolerance must be a positive finite number"
        );
    }

    if (maxIterations_ == 0) {
        throw std::invalid_argument(
            "Maximum iterations must be greater than zero"
        );
    }
}

OptimizationResult OptimizationTask::optimize(
    const std::function<double(double)>& function,
    double left,
    double right,
    double tolerance,
    OptimizationMode mode
) {
    ScopeLogger logger("optimize");

    if (!function) {
        throw std::invalid_argument(
            "Optimization function is empty"
        );
    }

    if (!std::isfinite(left) ||
        !std::isfinite(right) ||
        left >= right) {
        throw InvalidIntervalError(
            "Invalid search interval"
        );
    }

    if (!std::isfinite(tolerance) || tolerance <= 0.0) {
        throw InvalidToleranceError(
            "Tolerance must be a positive finite number"
        );
    }

    OptimizationResult newResult = search(
        function,
        left,
        right,
        tolerance,
        maxIterations_,
        mode
    );

    left_ = left;
    right_ = right;
    tolerance_ = tolerance;
    result_ = newResult;
    hasResult_ = true;

    return result_;
}

double OptimizationTask::getLeft() const {
    return left_;
}

double OptimizationTask::getRight() const {
    return right_;
}

double OptimizationTask::getTolerance() const {
    return tolerance_;
}

bool OptimizationTask::hasResult() const {
    return hasResult_;
}

OptimizationResult OptimizationTask::getResult() const {
    if (!hasResult_) {
        throw std::logic_error(
            "No optimization result is available"
        );
    }

    return result_;
}
