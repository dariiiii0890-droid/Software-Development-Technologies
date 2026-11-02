
#include "OptimizationTask.h"
#include "OptimizationExceptions.h"

#include <gtest/gtest.h>

#include <cmath>
#include <limits>
#include <stdexcept>

TEST(OptimizationTests, FindsMaximum) {
    OptimizationTask task(-5.0, 5.0, 0.000001);

    auto result = task.optimize(
        [](double x) {
            return -(x - 2.0) * (x - 2.0) + 5.0;
        },
        -5.0,
        5.0,
        0.000001,
        OptimizationMode::Maximum
    );

    EXPECT_NEAR(result.x, 2.0, 0.00001);
    EXPECT_NEAR(result.value, 5.0, 0.00001);
    EXPECT_GT(result.iterations, 0);
}

TEST(OptimizationTests, FindsMinimum) {
    OptimizationTask task(-5.0, 5.0, 0.000001);

    auto result = task.optimize(
        [](double x) {
            return (x + 1.0) * (x + 1.0);
        },
        -5.0,
        5.0,
        0.000001,
        OptimizationMode::Minimum
    );

    EXPECT_NEAR(result.x, -1.0, 0.00001);
    EXPECT_NEAR(result.value, 0.0, 0.00001);
}

TEST(OptimizationTests, FindsMinimumAtBoundary) {
    OptimizationTask task(0.0, 10.0, 0.000001);

    auto result = task.optimize(
        [](double x) {
            return x;
        },
        0.0,
        10.0,
        0.000001,
        OptimizationMode::Minimum
    );

    EXPECT_NEAR(result.x, 0.0, 0.00001);
}

TEST(OptimizationTests, FindsMaximumAtBoundary) {
    OptimizationTask task(0.0, 10.0, 0.000001);

    auto result = task.optimize(
        [](double x) {
            return x;
        },
        0.0,
        10.0,
        0.000001,
        OptimizationMode::Maximum
    );

    EXPECT_NEAR(result.x, 10.0, 0.00001);
}

TEST(OptimizationTests, RejectsInvalidConstructorInterval) {
    EXPECT_THROW(
        OptimizationTask(5.0, 1.0, 0.001),
        InvalidIntervalError
    );

    EXPECT_THROW(
        OptimizationTask(2.0, 2.0, 0.001),
        InvalidIntervalError
    );
}

TEST(OptimizationTests, RejectsNonFiniteInterval) {
    EXPECT_THROW(
        OptimizationTask(
            -std::numeric_limits<double>::infinity(),
            5.0,
            0.001
        ),
        InvalidIntervalError
    );
}

TEST(OptimizationTests, RejectsInvalidTolerance) {
    EXPECT_THROW(
        OptimizationTask(-5.0, 5.0, 0.0),
        InvalidToleranceError
    );

    EXPECT_THROW(
        OptimizationTask(-5.0, 5.0, -0.001),
        InvalidToleranceError
    );
}

TEST(OptimizationTests, RejectsZeroIterationLimit) {
    EXPECT_THROW(
        OptimizationTask(-5.0, 5.0, 0.001, 0),
        std::invalid_argument
    );
}

TEST(OptimizationTests, RejectsInvalidIntervalDuringOptimization) {
    OptimizationTask task(-5.0, 5.0, 0.001);

    EXPECT_THROW(
        task.optimize(
            [](double x) { return x * x; },
            5.0,
            -5.0,
            0.001
        ),
        InvalidIntervalError
    );
}

TEST(OptimizationTests, RejectsInvalidToleranceDuringOptimization) {
    OptimizationTask task(-5.0, 5.0, 0.001);

    EXPECT_THROW(
        task.optimize(
            [](double x) { return x * x; },
            -5.0,
            5.0,
            -0.001
        ),
        InvalidToleranceError
    );
}

TEST(OptimizationTests, RejectsNaNFunctionValue) {
    OptimizationTask task(-5.0, 5.0, 0.001);

    EXPECT_THROW(
        task.optimize(
            [](double) {
                return std::numeric_limits<double>::quiet_NaN();
            },
            -5.0,
            5.0,
            0.001
        ),
        InvalidFunctionValueError
    );
}

TEST(OptimizationTests, RejectsInfiniteFunctionValue) {
    OptimizationTask task(-5.0, 5.0, 0.001);

    EXPECT_THROW(
        task.optimize(
            [](double) {
                return std::numeric_limits<double>::infinity();
            },
            -5.0,
            5.0,
            0.001
        ),
        InvalidFunctionValueError
    );
}

TEST(OptimizationTests, RejectsEmptyFunction) {
    OptimizationTask task(-5.0, 5.0, 0.001);

    EXPECT_THROW(
        task.optimize(
            {},
            -5.0,
            5.0,
            0.001
        ),
        std::invalid_argument
    );
}

TEST(OptimizationTests, ThrowsWhenIterationLimitIsExceeded) {
    OptimizationTask task(-5.0, 5.0, 0.0000001, 1);

    EXPECT_THROW(
        task.optimize(
            [](double x) { return x * x; },
            -5.0,
            5.0,
            0.0000001
        ),
        IterationLimitError
    );
}

TEST(OptimizationTests, ThrowsWhenSearchCannotConverge) {
    OptimizationTask task(
        1e16,
        1e16 + 4.0,
        0.1,
        100
    );

    EXPECT_THROW(
        task.optimize(
            [](double x) { return x; },
            1e16,
            1e16 + 4.0,
            0.1
        ),
        NoConvergenceError
    );
}

TEST(OptimizationTests, PreservesPreviousStateAfterFailure) {
    OptimizationTask task(-5.0, 5.0, 0.001);

    auto firstResult = task.optimize(
        [](double x) { return (x - 2.0) * (x - 2.0); },
        -5.0,
        5.0,
        0.001,
        OptimizationMode::Minimum
    );

    EXPECT_THROW(
        task.optimize(
            [](double) {
                return std::numeric_limits<double>::quiet_NaN();
            },
            -10.0,
            10.0,
            0.0001
        ),
        InvalidFunctionValueError
    );

    auto savedResult = task.getResult();

    EXPECT_DOUBLE_EQ(task.getLeft(), -5.0);
    EXPECT_DOUBLE_EQ(task.getRight(), 5.0);
    EXPECT_DOUBLE_EQ(task.getTolerance(), 0.001);

    EXPECT_DOUBLE_EQ(savedResult.x, firstResult.x);
    EXPECT_DOUBLE_EQ(savedResult.value, firstResult.value);
    EXPECT_EQ(savedResult.iterations, firstResult.iterations);
}

TEST(OptimizationTests, ResultIsAvailableAfterSuccessfulSearch) {
    OptimizationTask task(-5.0, 5.0, 0.001);

    EXPECT_FALSE(task.hasResult());

    task.optimize(
        [](double x) { return x * x; },
        -5.0,
        5.0,
        0.001
    );

    EXPECT_TRUE(task.hasResult());
}

TEST(OptimizationTests, ThrowsWhenResultIsNotAvailable) {
    OptimizationTask task(-5.0, 5.0, 0.001);

    EXPECT_THROW(
        task.getResult(),
        std::logic_error
    );
}
