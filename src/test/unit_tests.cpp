#include <iostream>

#include "../include/core/database.hpp"

namespace Unit_tests {
    void basic_insert_test() {
        const std::string key = "1234";
        const std::vector<std::string> values = {"apple", "banana", "cherry"};
        
        Database::insert_entry(key, values);
        std::cout << "Inserted entry for key: " << key << std::endl;
    }

    // TODO: Add more test; lazy
}