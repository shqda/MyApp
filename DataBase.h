//
// Created by Артём Никитин on 13.11.2025.
//
#pragma once
#include <functional>
#include <sqlite3.h>
#include <string>
#include <vector>

#include "Employee.h"

class DataBase {
private:
    sqlite3* database_;

public:
    DataBase(const std::string& filename);

    void insert(const std::vector<Employee>& employees);
    bool add_employee(const Employee& e) noexcept;
    bool create_table() noexcept;
    void print_unique_sorted() noexcept;
    size_t count_by(std::function<bool(const Employee&)> predicate) noexcept;
    size_t optimized_count_by(const char* sql) noexcept;

    ~DataBase();
};


