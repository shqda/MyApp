//
// Created by Артём Никитин on 16.11.2025.
//
#pragma once
#include "Employee.h"
#include <functional>

class DataGenerator {
public:
    static std::vector<Employee> generate_employees(
        size_t count,
        std::optional<std::function<std::string()>> nameGen = std::nullopt,
        std::optional<std::function<std::string()>> birthdayGen = std::nullopt,
        std::optional<std::function<std::string()>> genderGen = std::nullopt
    ) noexcept;
};

