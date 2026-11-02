#pragma once

#include <string>

class ScopeLogger {
private:
    std::string name_;

public:
    explicit ScopeLogger(const std::string& name);
    ~ScopeLogger() noexcept;

    ScopeLogger(const ScopeLogger&) = delete;
    ScopeLogger& operator=(const ScopeLogger&) = delete;
};