#include "../include/frontend/window.hpp"
#include "../include/core/database.hpp"
#include <iostream>

namespace Database_service{
    int run(int argc, char* argv[]) {
    
    if (!Window::initialize()) {
        std::cout << "Failed to initialize SDL window" << std::endl;
        return -1;
    }

    const std::string& database_file_path = Window::get_database_file_path();
    const std::string& index_file_path = Window::get_index_file_path();

    Database::init_db(database_file_path, index_file_path);


    const std::string key = "1234";
    const std::vector<std::string> values = {"apple", "banana", "cherry"};
    
    // Create
    Database::insert_entry(key, values);
    std::cout << "Inserted entry for key: " << key << std::endl;

    
    Window::run();

    Database::cleanup_db();
    Window::cleanup();

    return 0;
}

}
