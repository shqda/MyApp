#include <iostream>
#include <random>

#include "DataBase.h"
#include "DataGenerator.h"
#include <chrono>

void usage() {
    std::cout << "MyApp 1 : create table\n"
            << "MyApp 2 : add new employee (\"Full name\" Birth_date(YYYY-MM-DD) Gender(Male/Female))\n"
            << "MyApp 3 : print unique employees\n"
            << "MyApp 4 : fill table with 1`000`000 random workers\n"
            << "MyApp 5 : count males with surname starting with 'F'\n"
            << "MyApp 6 : count males with surname starting with 'F' (faster version)\n";
}

auto print_execution_time(const std::string &description, const std::function<size_t()> &func) {
    auto start = std::chrono::high_resolution_clock::now();
    auto result = func();
    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << description << ": " << duration.count() << " microseconds.\n";

    return result;
}


int main(int argc, char *argv[]) {
    if (argc < 2) {
        usage();
        return 0;
    }

    DataBase db("employees.sqlite");

    int number;
    try {
        number = std::stoi(argv[1]);
    } catch (std::invalid_argument &e) {
        std::cout << "execution argument error: " << e.what() << std::endl;
        return 1;
    }

    switch (number) {
        case 1: {
            db.create_table();
            break;
        }
        case 2: {
            if (argc < 5) {
                usage();
                break;
            };
            try {
                db.add_employee({argv[2], argv[3], argv[4]});
            }
            catch (std::invalid_argument& e) {
                std::cout << "execution argument error: " << e.what() << std::endl;
            }
            break;
        }
        case 3: {
            db.print_unique_sorted();
            break;
        }
        case 4: {
            db.insert(DataGenerator::generate_employees(1000000));
            db.insert(DataGenerator::generate_employees(100,
                [] { return Employee::generate_full_name('F'); },
                std::nullopt,
                [] { return Employee::generate_gender(Gender::Male); })
            );
            break;
        }
        case 5: {
            std::cout << print_execution_time("count_by()", [&db] {
                return db.count_by([](const Employee& e) {
                    return e.get_full_name()[0] == 'F' && e.get_gender() == "Male";
                });
            });
            break;
        }
        case 6: {
            const char *sql = "SELECT COUNT(*) FROM employees WHERE full_name LIKE 'F%' AND gender = 'Male';";
            std::cout << print_execution_time("optimized_count_by()", [&db, &sql] {
                return db.optimized_count_by(sql);
            });
            break;
        }
        default:
            usage();
    }
    return 0;
}
