//
// Created by Артём Никитин on 16.11.2025.
//

#include "DataGenerator.h"

std::vector<Employee> DataGenerator::generate_employees(
    size_t count,
    std::optional<std::function<std::string()> > nameGen,
    std::optional<std::function<std::string()> > birthdayGen,
    std::optional<std::function<std::string()> > genderGen
    ) noexcept {

    std::vector<Employee> employees;
    employees.reserve(count);

    auto defaultNameGen = []{ return Employee::generate_full_name(); };
    auto defaultBirthdayGen = []{ return Employee::generate_birthday(); };
    auto defaultGenderGen = []{ return Employee::generate_gender(); };

    for (size_t i = 0; i < count; i++) {
        employees.emplace_back(
            nameGen.value_or(defaultNameGen)(),
            birthdayGen.value_or(defaultBirthdayGen)(),
            genderGen.value_or(defaultGenderGen)()
        );
    }

    return employees;
}
