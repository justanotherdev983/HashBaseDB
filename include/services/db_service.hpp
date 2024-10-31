#pragma once

#include <iostream>
#include <vector>

#include "frontend/window.hpp"
#include "core/database.hpp"
#include "test/unit_tests.hpp"
#include "services/ui_service.hpp"

namespace Database_service{
    int run (int argc, char* argv[]);
    std::vector<std::string> search_database();
    void add_record(const std::string& key, const std::vector<std::string>& values);
}
