#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include "../include/database.hpp"

int main() {
    try {
        // Print current working directory
        std::cout << "Current working directory: " << std::filesystem::current_path() << std::endl;
        
        // Use relative path from current directory
        Database::init_db("../database/winkel.db", "../database/index.idx");
        
        const std::string key = "12345";
        const std::vector<std::string> values = {"value1", "value2", "value3"};

        Database::insert_entry(key, values);
        std::vector<std::string> retrieved_values = Database::get_val(key);

        std::cout << "Retrieved values for key " << key << ":\n";
        for (const auto& value : retrieved_values) {
            std::cout << value << "\n";
        }
        
        Database::cleanup_db();
    }
    catch (const std::exception& e) {
        std::cout << "Program failed: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}