//
// Created by Артём Никитин on 13.11.2025.
//
#include <chrono>
#include "Employee.h"
std::random_device rd;
std::mt19937 GEN(rd());


char Employee::random_uppercase_letter() {
    static std::uniform_int_distribution<int> dist('A', 'Z');
    return static_cast<char>(dist(GEN));
}

std::string Employee::random_string(size_t len) {
    static std::uniform_int_distribution<int> dist('a', 'z');
    std::string s;
    s.reserve(len);

    for (int i = 0; i < len; i++) {
        s.push_back(static_cast<char>(dist(GEN)));
    }
    return s;
}

std::string Employee::generate_full_name(std::optional<char> first_letter) {
    static std::uniform_int_distribution<int> name_len(2, 10);

    std::string surname;
    std::string name = random_uppercase_letter() + random_string(name_len(GEN));
    if (first_letter) {
        surname = *first_letter + random_string(name_len(GEN));
    }
    else {
        surname = random_uppercase_letter() + random_string(name_len(GEN));
    }
    std::string patronymic = random_uppercase_letter() + random_string(name_len(GEN));

    return surname + " " + name + " " + patronymic;
}




std::string Employee::generate_birthday() {
    std::uniform_int_distribution year(1950, 2010);
    std::uniform_int_distribution month(1, 12);
    std::uniform_int_distribution day(1, 28);
    return std::to_string(year(GEN)) + '-' +
           std::to_string(month(GEN)) + '-' +
           std::to_string(day(GEN));
}

std::string Employee::generate_gender(std::optional<Gender> g) {
    std::uniform_int_distribution gender(0, 1);
    if (!g) {
        return gender(GEN) == 0 ? "Male" : "Female";
    }
    else return g ? "Male" : "Female";
}


bool Employee::check_if_birthday_valid(const std::string& birthday) noexcept {
    std::stringstream ss(birthday);
    int year, month, day;
    char sep1, sep2;
    if (!(ss >> year >> sep1 >> month >> sep2 >> day)) return false;

    if (sep1 != '-' || sep2 != '-') return false;

    if (year < 0 || year > 2025) return false;
    if (month < 1 || month > 12) return false;
    if (day < 1 || day > 31) return false;

    return true;
}


Employee::Employee(
    const std::string &full_name,
    const std::string &birthday,
    const std::string &gender)
{
    if (full_name.empty()) {
        throw std::invalid_argument("Name cannot be empty");
    }
    if (birthday.empty()) {
        throw std::invalid_argument("Birth date cannot be empty");
    }
    if (!check_if_birthday_valid(birthday)) {
        throw std::invalid_argument("Birth date not valid");
    }
    if (gender != "Male" && gender != "Female") {
        throw std::invalid_argument("Gender must be Male or Female");
    }
    this->full_name_ = full_name;
    this->birthday_ = birthday;
    this->gender_ = gender;
}

size_t Employee::calculate_age() const {
    int year, month, day;
    char sep1, sep2;

    std::stringstream ss(birthday_);
    ss >> year >> sep1 >> month >> sep2 >> day;
    if (!ss || sep1 != '-' || sep2 != '-') return 0;

    std::time_t t = std::time(nullptr);
    std::tm* now = std::localtime(&t);

    int age = (now->tm_year + 1900) - year;

    if ((now->tm_mon + 1 < month) || ((now->tm_mon + 1 == month) && (now->tm_mday < day))) {
        age--;
    }

    return age;
}
