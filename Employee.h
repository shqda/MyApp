//
// Created by Артём Никитин on 13.11.2025.
//
#pragma once
#include <string>
#include <random>
#include <optional>

enum class Gender : uint8_t {
    Male = 0,
    Female = 1,
};


class Employee {
private:
    std::string full_name_;
    std::string birthday_;
    std::string gender_;

    static char random_uppercase_letter();
    static std::string random_string(size_t len);
    static bool check_if_birthday_valid(const std::string& birthday) noexcept;
public:
    static std::string generate_full_name(std::optional<char> first_letter = std::nullopt);
    static std::string generate_birthday();
    static std::string generate_gender(::std::optional<Gender> g = std::nullopt);

    Employee() = default;
    Employee(
        const std::string& full_name,
        const std::string& birthday,
        const std::string& gender
    );

    size_t calculate_age() const;

    const std::string& get_full_name() const noexcept { return full_name_;}
    const std::string& get_birthday() const noexcept { return birthday_;}
    const std::string& get_gender() const noexcept { return gender_;}
};

