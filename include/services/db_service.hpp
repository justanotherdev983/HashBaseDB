#pragma once

#include "../include/frontend/window.hpp"

namespace Database_service{
    int run (int argc, char* argv[]);
    std::vector<std::string> search_database();
    void add_record(const std::string& key, const std::vector<std::string>& values);
}
