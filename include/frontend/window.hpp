#pragma once
#include <string>
#include <SDL.h>

namespace Window {
    bool initialize();
    void run();
    void cleanup();

    std::string get_database_file_path();
    std::string get_index_file_path();
}