#include <fstream>
#include <iostream>
#include <string>
#include <cstdint>
#include <limits>
#include <cstring>
#include <vector>
#include <sstream>
#include <filesystem>

#include "core/hash.hpp" 
#include "core/database.hpp"

namespace Database {

    std::fstream index_file;
    std::fstream database_file;

    constexpr size_t NUM_BUCKETS = 100;
    constexpr size_t BUCKET_SIZE = 64;
    constexpr const char* SEPARATOR = ", ";


    void init_db(const std::string& db_path, const std::string& index_path) {
        // Close any open files first
        if (index_file.is_open()) index_file.close();
        if (database_file.is_open()) database_file.close();

        std::filesystem::path db_absolute = std::filesystem::absolute(db_path);
        std::filesystem::path idx_absolute = std::filesystem::absolute(index_path);

        std::filesystem::path dir_path = db_absolute.parent_path();
        if (!std::filesystem::exists(dir_path)) {
            std::filesystem::create_directories(dir_path);
        }

        // Now open both files for reading and writing
        index_file.open(idx_absolute, std::ios::in | std::ios::out | std::ios::app);
        if (!index_file) {
            throw std::runtime_error("Cannot open index file for R/W: " + std::string(std::strerror(errno)));
        }
        index_file.clear();

        // Initialize the index file with empty buckets
        for (size_t i = 0; i < NUM_BUCKETS * BUCKET_SIZE; ++i) {
            //index_file << "0, 0\n";
        }
        index_file.flush();

        database_file.open(db_absolute, std::ios::in | std::ios::out | std::ios::app);
        if (!database_file) {
            throw std::runtime_error("Cannot open database file for R/W: " + std::string(std::strerror(errno)));
        }

        // Verify file size
        index_file.seekg(0, std::ios::end);
        std::streampos file_size = index_file.tellg();
        index_file.clear();
        index_file.seekg(0);

        std::cout << "Files created and opened successfully." << std::endl;
        std::cout << "Database initialized with:\n"
                << "- Number of buckets: " << NUM_BUCKETS << "\n"
                << "- Bucket size: " << BUCKET_SIZE << " bytes\n"
                << "- Total index size: " << file_size << " bytes" << std::endl;
    }

    void cleanup_db() {
        if (database_file.is_open()) {
            database_file.close();
            std::cout << "Database file closed." << std::endl;
        }
        if (index_file.is_open()) {
            index_file.close();
            std::cout << "Index file closed." << std::endl;
        }
    }

    std::vector<std::string> get_val(const std::string& id_from_user) {
        std::vector<std::string> values;

        try {
            uint64_t hashed_key = Hash::hashify(id_from_user);

            // Debugging bucket calculation
            size_t bucket = Hash::get_bucket(hashed_key, NUM_BUCKETS);
            int64_t bucket_position = Hash::calculate_bucket_position(bucket, BUCKET_SIZE, NUM_BUCKETS);

            std::cout << "UUID: " << id_from_user << std::endl;
            std::cout << "Hashed key: " << hashed_key << ", Bucket: " << bucket << ", Position: " << bucket_position << std::endl;

            // Open the index file and read lines
            index_file.clear();
            index_file.seekg(bucket_position);

            std::string line;
            size_t lines_read = 0;
            while (std::getline(index_file, line) && lines_read < BUCKET_SIZE) {
                std::cout << "Index line: '" << line << "'" << std::endl;  // Debugging line reading
                if (line.empty() || line == "0, 0") {
                    ++lines_read;
                    continue;
                }

                std::istringstream iss(line);
                uint64_t temp_key;
                long int offset;
                char comma;

                if (!(iss >> temp_key >> comma >> offset) || comma != ',') {
                    std::cout << "Skipping malformed index line: " << line << std::endl;
                    ++lines_read;
                    continue;
                }

                if (temp_key == hashed_key) {
                    std::cout << "Found matching index line, seeking to offset: " << offset << std::endl;

                    // Seek to the database file at the matched offset
                    database_file.clear();
                    database_file.seekg(offset);

                    std::string db_line;
                    if (std::getline(database_file, db_line)) {
                        std::cout << "Database line at offset " << offset << ": " << db_line << std::endl;
                        size_t sep_pos = db_line.find(SEPARATOR);
                        if (sep_pos != std::string::npos) {
                            values.push_back(db_line.substr(sep_pos + strlen(SEPARATOR)));
                        } else {
                            std::cout << "Separator not found in database line: " << db_line << std::endl;
                        }
                    } else {
                        std::cout << "Failed to read database line at offset " << offset << std::endl;
                    }
                }
                ++lines_read;
            }
        } catch (const std::runtime_error& e) {
            std::cout << "Runtime error: " << e.what() << std::endl;
        }

        if (values.empty()) {
            std::cout << "No records found for UUID: " << id_from_user << std::endl;
        }

        return values;
    }

    void insert_entry(const std::string& key, const std::vector<std::string>& values) {
        Hash::debug_hash(key, NUM_BUCKETS, BUCKET_SIZE);
        
        database_file.clear();
        index_file.clear();

        // First write to database file
        database_file.seekp(0, std::ios::end);
        long data_offset = database_file.tellp();

        // Prepare the database entry
        std::stringstream data_line;
        uint64_t hash_key = Hash::hashify(key);
        data_line << hash_key << SEPARATOR;
        for (size_t i = 0; i < values.size(); ++i) {
            data_line << values[i];
            if (i < values.size() - 1) {
                data_line << SEPARATOR;
            }
        }
        data_line << "\n";

        // Write to database file
        database_file << data_line.str();
        database_file.flush();

        // Now handle the index file
        uint64_t bucket = Hash::get_bucket(hash_key, NUM_BUCKETS);
        long bucket_pos = bucket * BUCKET_SIZE;
        
        // Ensure the index file is large enough
        index_file.seekp(0, std::ios::end);
        long file_size = index_file.tellp();
        
        if (file_size < bucket_pos) {
            // Fill the gap with empty space
            std::string padding(bucket_pos - file_size, '\n');
            index_file << padding;
        }
        
        // Write the index entry
        index_file.seekp(bucket_pos);
        index_file << hash_key << SEPARATOR << data_offset << "\n";
        index_file.flush();
    }

    void remove_entry(const std::string& key) {
        index_file.clear();
        index_file.seekg(0, std::ios::beg);

        uint64_t temp_key;
        long offset;
        char comma;
        uint64_t hashed_key = Hash::hashify(key);
        
        std::vector<std::string> lines;
        std::string line;
        bool found = false;
        while (std::getline(index_file, line)) {
            if (line.empty() || std::all_of(line.begin(), line.end(), isspace)) {
                continue;
            }

            std::istringstream iss(line);
            if (iss >> temp_key >> comma >> offset) {
                if (temp_key == hashed_key) {
                    found = true;
                    line = "d" + std::to_string(temp_key) + SEPARATOR + std::to_string(offset);
                }
            } else {
                std::cout << "Warning: Skipping malformed line: " << line << std::endl;
            }
            lines.push_back(line);
        }

        // Reopen the index file in write mode, truncating it
        index_file.close();  
        index_file.open("../database/index.idx", std::ios::out | std::ios::app);  

        for (const auto& l : lines) {
            index_file << l << std::endl;
        }

        index_file.flush();  // Ensure everything is written

        if (found) {
            std::cout << "Marked key " << key << " as deleted." << std::endl;
        } else {
            std::cout << "Key " << key << " not found." << std::endl;
        }
    }

    void debug_print_files() {
        std::cout << "\nDatabase contents:\n";
        database_file.clear();
        database_file.seekg(0);
        std::string line;

        while (std::getline(database_file, line)) {
            std::cout << line << '\n';
        }
        
        std::cout << "\nIndex contents:\n";
        index_file.clear();
        index_file.seekg(0);

        while (std::getline(index_file, line)) {
            std::cout << line << '\n';
        }
        std::cout << std::endl;
    }

    void debug_file_stream(std::fstream& file, const std::string& operation_name) {
        std::cout << "\n=== Stream Debug for operation: " << operation_name << " ===\n";
        std::cout << "Stream state:\n";
        std::cout << "- eof():     " << file.eof() << "\n";
        std::cout << "- fail():    " << file.fail() << "\n";
        std::cout << "- bad():     " << file.bad() << "\n";
        std::cout << "- good():    " << file.good() << "\n";
        
        // Try to get position
        file.clear(); // Clear error flags
        std::streampos pos = file.tellg();
        std::cout << "Current position: " << (pos == -1 ? "invalid (-1)" : std::to_string(pos)) << "\n";
        std::cout << "File is " << (file.is_open() ? "open" : "closed") << "\n";
        std::cout << "=====================================\n\n";
    }
}
