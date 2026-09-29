#pragma once

#include <memory>
#include "shared_types.h"

// Студент А: рекурсивний розклад визначника за елементами рядка (метод Лапласа)
std::unique_ptr<Result> calculateA(std::shared_ptr<const InputData> data);