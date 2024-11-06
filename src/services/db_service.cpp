#include <iostream>
#include <vector>

#include "core/database.hpp"
#include "frontend/window.hpp"
#include "services/ui_service.hpp"
#include "test/unit_tests.hpp"


namespace Database_service {
    int run(int argc, char *argv[]) {
        if (!Window::initialize()) {
            std::cout << "Failed to initialize SDL window" << std::endl;
            return -1;
        }

        const std::string &database_file_path = UI::get_database_file_path();
        const std::string &index_file_path = UI::get_index_file_path();

        Database::init_db(database_file_path, index_file_path);

        // Unit_tests::basic_insert_test();

        Window::initialize();
        Window::run();

        Database::cleanup_db();
        Window::cleanup();

        return 0;
    }

    std::vector<std::string> search_database(const std::string &key) {
        Database::database_file.clear();
        Database::database_file.seekg(0);
        Database::index_file.clear();
        Database::index_file.seekg(0);

        std::vector<std::string> results = Database::get_val(key);
        if (results.empty()) {
            std::cout << "No records found for UUID: " << key << std::endl;
        } else {
            std::cout << "Records found for UUID " << key << ":" << std::endl;
            for (const auto &result : results) {
                std::cout << result << std::endl;
            }
        }
        return results;
    }

    void add_record(const std::string &key, const std::vector<std::string> &values) {
        Database::database_file.seekp(0, std::ios::end);
        Database::insert_entry(key, values);
    }

    void remove_record(const std::string &key) {
        Database::remove_entry(key);
    }
}

/*

std::vector<std::string> search_database(const std::string& key) {
        std::vector<std::string> results;
        Database::database_file.clear();
        Database::database_file.seekg(0);
        Database::index_file.clear();
        Database::index_file.seekg(0);
        std::string db_line;
        std::string idx_line;

        uint64_t hashed_key = Hash::hashify(key);
        std::string hashed_key_str = std::to_string(hashed_key);

        size_t i = 0;
        while (std::getline(Database::database_file, db_line)) {
            std::getline(Database::index_file, idx_line);

            if (idx_line.rfind("d", 0) == 0) {
                continue;
            }
            // Check if the line starts with the hashed key
            if (db_line.rfind(hashed_key_str + ", ", 0) == 0) { // Asserting ',
' is the separator results.push_back(db_line.substr(hashed_key_str.size() + 2));
// Skip the hashed key and separator
            }
            ++i;
        }
        return results;
    }

*/