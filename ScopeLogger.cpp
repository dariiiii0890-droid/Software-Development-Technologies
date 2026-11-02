#include "ScopeLogger.h"

#include <iostream>

ScopeLogger::ScopeLogger(const std::string& name)
    : name_(name) {
    std::cout << "[ENTER] " << name_ << '\n';
}

ScopeLogger::~ScopeLogger() noexcept {
    try {
        std::cout << "[EXIT] " << name_ << '\n';
    }
    catch (...) {
    }
}