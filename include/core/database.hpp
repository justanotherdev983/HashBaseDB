#pragma once

#include "../include/core/hash.hpp" 
#include <fstream>
#include <iostream>

namespace Database {
    extern std::fstream index_file;
    extern std::fstream database_file;

    void init_db(const std::string& db_file, const std::string& index_file);
    void cleanup_db();
    void insert_entry(const std::string& key, const std::vector<std::string>& values);
    void remove_entry(const std::string& key);
    std::vector<std::string> get_val(const std::string& id_from_user);
    void print_debug_info();
}
