//
// Created by Артём Никитин on 13.11.2025.
//

#include "DataBase.h"

#include <functional>
#include <iomanip>
#include <iostream>
#include <stdexcept>


DataBase::DataBase(const std::string &filename) : database_(nullptr) {
    if (sqlite3_open(filename.c_str(), &database_) != SQLITE_OK) {
        const char* msg = sqlite3_errmsg(database_);
        sqlite3_close(database_);
        database_ = nullptr;
        throw std::runtime_error(std::string("Can't open database: ") + msg);
    }
}


bool DataBase::create_table() noexcept {
    const char* sql =
        "CREATE TABLE IF NOT EXISTS employees ("
        " id INTEGER PRIMARY KEY AUTOINCREMENT,"
        " full_name   TEXT NOT NULL,"
        " birth  TEXT NOT NULL,"
        " gender TEXT NOT NULL"
        ");";

    char* errorMsg = nullptr;

    if (sqlite3_exec(database_, sql, nullptr, nullptr, &errorMsg) != SQLITE_OK) {
        std::cerr << "SQLite error: " << errorMsg << std::endl;
        sqlite3_free(errorMsg);
        return false;
    }

    return true;
}

DataBase::~DataBase() {
    if (database_) {
        sqlite3_close(database_);
        database_ = nullptr;
    }
}

void DataBase::print_unique_sorted() noexcept {
    const char* sql =
        "SELECT full_name, birth, gender "
        "FROM Employees "
        "GROUP BY full_name, birth "
        "ORDER BY full_name;";

    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(database_, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "SQL error: " << sqlite3_errmsg(database_) << std::endl;
        sqlite3_finalize(stmt);
        return;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string full_name  = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        std::string birthday = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        std::string gender     = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));

        Employee e(full_name, birthday, gender);

        std::cout
            << std::left << std::setw(25) << full_name
            << " | "
            << std::setw(10) << birthday
            << " | "
            << std::setw(6)  << gender
            << " | age: "
            << std::setw(3)  << e.calculate_age()
            << '\n';
    }
    sqlite3_finalize(stmt);
}

size_t DataBase::count_by(std::function<bool(const Employee&)> predicate) noexcept {
    const char* sql = "SELECT full_name, birth, gender FROM employees;";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(database_, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "SQL error: " << sqlite3_errmsg(database_) << "\n";
        return 0;
    }

    size_t count = 0;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Employee e(
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)),
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)),
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2))
        );

        if (predicate(e))
            count++;
    }

    sqlite3_finalize(stmt);
    return count;
}

//this version of counting employees method is better because of sql is always faster than C++
size_t DataBase::optimized_count_by(const char* sql) noexcept {
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(database_, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "SQL error: " << sqlite3_errmsg(database_) << std::endl;
        sqlite3_finalize(stmt);
        return 0;
    }
    size_t count = 0;

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        count = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);
    return count;
}

void DataBase::insert(const std::vector<Employee>& employees) {
    char* err = nullptr;

    sqlite3_exec(database_, "BEGIN TRANSACTION;", nullptr, nullptr, &err);

    const char* zSql = "INSERT INTO Employees (full_name, birth, gender) VALUES (?, ?, ?);";

    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(database_, zSql, -1, &stmt, nullptr);

    for (const auto& e : employees) {
        sqlite3_bind_text(stmt, 1, e.get_full_name().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, e.get_birthday().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, e.get_gender().c_str(), -1, SQLITE_TRANSIENT);

        sqlite3_step(stmt);
        sqlite3_reset(stmt);
        sqlite3_clear_bindings(stmt);
    }
    sqlite3_finalize(stmt);
    sqlite3_exec(database_, "COMMIT;", nullptr, nullptr, &err);
}



bool DataBase::add_employee(const Employee &e) noexcept {
    const char* sql =
            "INSERT INTO employees (full_name, birth, gender)"
            "VALUES (?, ?, ?);";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(database_, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "SQL error: " << sqlite3_errmsg(database_) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }
    sqlite3_bind_text(stmt, 1, e.get_full_name().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, e.get_birthday().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, e.get_gender().c_str(), -1, SQLITE_TRANSIENT);

    bool ok = sqlite3_step(stmt) == SQLITE_DONE;

    sqlite3_finalize(stmt);

    return ok;
}
