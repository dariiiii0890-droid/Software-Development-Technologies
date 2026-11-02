
#pragma once

#include <stdexcept>
#include <string>

class OptimizationError : public std::runtime_error {
public:
    explicit OptimizationError(const std::string& message)
        : std::runtime_error(message) {}
};

class InvalidIntervalError : public OptimizationError {
public:
    explicit InvalidIntervalError(const std::string& message)
        : OptimizationError(message) {
    }
};

class InvalidToleranceError : public OptimizationError {
public:
    explicit InvalidToleranceError(const std::string& message)
        : OptimizationError(message) {
    }
};

class InvalidFunctionValueError : public OptimizationError {
public:
    explicit InvalidFunctionValueError(const std::string& message)
        : OptimizationError(message) {
    }
};

class NoConvergenceError : public OptimizationError {
public:
    explicit NoConvergenceError(const std::string& message)
        : OptimizationError(message) {
    }
};

class IterationLimitError : public OptimizationError {
public:
    explicit IterationLimitError(const std::string& message)
        : OptimizationError(message) {
    }
};
