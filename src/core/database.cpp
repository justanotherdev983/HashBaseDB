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
            index_file << "0, 0\n";
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

        // Initial debug check
        debug_file_stream(index_file, "Initial state");

        try {
            Hash::debug_hash(id_from_user, NUM_BUCKETS, BUCKET_SIZE);
            uint64_t hashed_key = Hash::hashify(id_from_user);
            size_t bucket = Hash::get_bucket(hashed_key, NUM_BUCKETS);

            std::streampos bucket_pos = static_cast<std::streampos>(bucket * BUCKET_SIZE);
            std::cout << "Searching bucket " << bucket << " at position " << bucket_pos << std::endl;

            // Check file size and seek position
            index_file.clear();
            index_file.seekg(0, std::ios::end);
            debug_file_stream(index_file, "After seeking to end");
            std::streampos file_size = index_file.tellg();

            if (file_size < 0) {
                throw std::runtime_error("Could not determine file size");
            }

            if (bucket_pos >= file_size) {
                std::cout << "Bucket position " << bucket_pos << " is beyond file size " << file_size << std::endl;
                return values;
            }

            // Seek to the bucket position
            index_file.clear();
            index_file.seekg(bucket_pos);
            debug_file_stream(index_file, "After seeking to bucket position");

            if (!index_file.good()) {
                throw std::runtime_error("Failed to seek to bucket position");
            }

            // Process the index file
            database_file.clear();
            debug_file_stream(database_file, "Database file initial state");

            std::string line;
            while (std::getline(index_file, line) && !line.empty()) {
                std::cout << "Reading index line: " << line << std::endl;

                // Skip deleted entries
                if (line[0] == 'd') {
                    std::cout << "Skipping deleted entry" << std::endl;
                    continue;
                }

                if(line == "0, 0") {
                    std::cout << "Skipping empty entry" << std::endl;
                    break;
                }

                // Check if the line can be parsed correctly
                try {
                    std::istringstream iss(line);
                    uint64_t temp_key;
                    long int offset;
                    char comma;

                    if (!(iss >> temp_key >> comma >> offset)) {
                        throw std::runtime_error("Failed to parse index line: " + line);
                    }

                    if (temp_key == hashed_key) {
                        std::cout << "Found matching key in index! Offset: " << offset << std::endl;

                        // Read from the database file
                        database_file.clear();
                        database_file.seekg(offset);
                        debug_file_stream(database_file, "After seeking to database offset");

                        std::string db_line;
                        if (std::getline(database_file, db_line)) {
                            std::cout << "Read database line: " << db_line << std::endl;

                            size_t sep_pos = db_line.find(SEPARATOR);
                            if (sep_pos != std::string::npos) {
                                values.push_back(db_line.substr(sep_pos + strlen(SEPARATOR)));
                                std::cout << db_line << std::endl;
                                return values;  // Found our entry
                                //break;  // Found our entry
                            } else {
                                throw std::runtime_error("Separator not found in database line");
                            }
                        } else {
                            throw std::runtime_error("Failed to read database line at offset");
                        }
                    }
                } catch (const std::runtime_error& e) {
                    std::cout << "Error processing index line: " << e.what() << std::endl;
                    // Skip the line if it's malformed and continue
                    continue;
                }
            }
        } catch (const Hash::HashingError& e) {
            std::cout << "Hashing error: " << e.what() << std::endl;
        } catch (const Hash::BucketError& e) {
            std::cout << "Bucket error: " << e.what() << std::endl;
        } catch (const std::runtime_error& e) {
            std::cout << "Runtime error: " << e.what() << std::endl;
        }

        // Final state check
        debug_file_stream(index_file, "Final index file state");
        debug_file_stream(database_file, "Final database file state");

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
