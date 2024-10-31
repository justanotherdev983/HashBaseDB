#include <iostream>
#include <vector>

#include "frontend/window.hpp"
#include "core/database.hpp"
#include "test/unit_tests.hpp"
#include "services/ui_service.hpp"

namespace Database_service{
    int run(int argc, char* argv[]) {

        if (!Window::initialize()) {
            std::cout << "Failed to initialize SDL window" << std::endl;
            return -1;
        }

        const std::string& database_file_path = UI::get_database_file_path();
        const std::string& index_file_path = UI::get_index_file_path();

        Database::init_db(database_file_path, index_file_path);

        //Unit_tests::basic_insert_test();

        
        Window::run();

        Database::cleanup_db();
        Window::cleanup();

        return 0;
    }

    std::vector<std::string> search_database() {
        std::vector<std::string> results;
        Database::database_file.clear();
        Database::database_file.seekg(0);
        std::string line;

        while (std::getline(Database::database_file, line)) {
            results.push_back(line);
        }

        Database::index_file.clear();
        Database::index_file.seekg(0);

        while (std::getline(Database::index_file, line)) {
            results.push_back(line);
        }

        return results;
    }

    void add_record(const std::string& key, const std::vector<std::string>& values) {
        Database::insert_entry(key, values);
    }
}

