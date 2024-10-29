#include <iostream>
#include <vector>
#include <string>
#include <filesystem>

#include "../include/core/database.hpp"
#include "../include/frontend/window.hpp"
#include "../include/services/db_service.hpp"

int main(int argc, char* argv[]) {
    Database_service::run(argc, argv);
    return 0;
}
