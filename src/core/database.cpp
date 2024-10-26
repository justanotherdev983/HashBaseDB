#include "../include/hash.hpp" 
#include <fstream>
#include <iostream>
#include <string>
#include <cstdint>
#include <limits>
#include <cstring>
#include <vector>
#include <sstream>
#include <filesystem>

namespace Database {

    std::fstream index_file;
    std::fstream database_file;

    constexpr size_t NUM_BUCKETS = 1000;
    constexpr size_t BUCKET_SIZE = 640;
    constexpr const char* SEPARATOR = ", ";


    void init_db(const std::string& db_path, const std::string& index_path) {
        // Print absolute paths
        std::filesystem::path db_absolute = std::filesystem::absolute(db_path);
        std::filesystem::path idx_absolute = std::filesystem::absolute(index_path);
        

        std::filesystem::path dir_path = db_absolute.parent_path();
        if (!std::filesystem::exists(dir_path)) {
            std::cout << "Directory does not exist: " << dir_path << std::endl;
            std::filesystem::create_directories(dir_path);
            std::cout << "Created directory" << std::endl;
        }
       
        std::ofstream db_create(db_absolute, std::ios::out | std::ios::app);
        if (!db_create.is_open()) {
            throw std::runtime_error("Cannot create database file: " + std::string(std::strerror(errno)));
        }
        db_create.close();

        std::ofstream idx_create(idx_absolute, std::ios::out | std::ios::app);
        if (!idx_create.is_open()) {
            throw std::runtime_error("Cannot create index file: " + std::string(std::strerror(errno)));
        }
        idx_create.close();

        database_file.open(db_absolute, std::ios::app | std::ios::in | std::ios::out);
        if (!database_file.is_open()) {
            throw std::runtime_error("Cannot open database file for R/W: " + std::string(std::strerror(errno)));
        }

        index_file.open(idx_absolute, std::ios::app | std::ios::in | std::ios::out);
        if (!index_file.is_open()) {
            throw std::runtime_error("Cannot open index file for R/W: " + std::string(std::strerror(errno)));
        }

        std::cout << "Files created and opened successfully." << std::endl;
    
    
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
        uint64_t hashed_key = Hash::hashify(id_from_user);
        std::vector<std::string> values;
        uint64_t temp_key;
        long offset;
        char comma;
        uint64_t bucket = Hash::get_bucket(hashed_key, NUM_BUCKETS);
        
        index_file.clear();
        index_file.seekg(bucket * BUCKET_SIZE, std::ios::beg);
        
        while (index_file >> temp_key >> comma >> offset) {
            
            if (index_file.peek() == 'd') {
                index_file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Encountered deleted key" << std::endl;
                continue;
            }
            
            if (temp_key == hashed_key) {
                
                
                database_file.clear();
                database_file.seekg(offset, std::ios::beg);
                
                std::string line;
                std::getline(database_file, line);
                
                std::istringstream iss(line); // size of SEPARATOR
                iss >> offset;
                
                
                size_t sep_pos = line.find(SEPARATOR);
                if (sep_pos != std::string::npos) {
                    values.push_back(line.substr(sep_pos + strlen(SEPARATOR)));
                }
            }
            
            index_file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        return values;
    }

    void insert_entry(const std::string& key, const std::vector<std::string>& values) {
        database_file.clear();
        index_file.clear();

        std::cout << "Starting insertion \n" << "Key is: "<< key << std::endl;
        database_file.seekp(0, std::ios::end);
        long offset = database_file.tellp();

        std::stringstream line;
        uint64_t hash_key = Hash::hashify(key);
        
        line << hash_key << SEPARATOR;

        for (size_t i = 0; i < values.size(); ++i) {
            line << values[i];
            std::cout << values[i] << std::endl;
            
            if (i < values.size() - 1) {
                line << SEPARATOR;
            }
        }

        line << "\n";
        std::cout << line.str() << std::endl;

        if (database_file << line.str()) {
            std::cout << "Inserted key and value into database succesfully" << std::endl;
        } else {
            std::cout << "Error writing to database_file!" << std::endl;
        }

        uint64_t hashed_key = Hash::hashify(key);
        uint64_t bucket = Hash::get_bucket(hashed_key, NUM_BUCKETS);

        index_file.seekp(bucket * BUCKET_SIZE, std::ios::beg); // Find the correct bucket
        
        if (index_file << hashed_key << SEPARATOR << offset << std::endl) {
            std::cout << "Inserted key and offset into index succesfully" << std::endl;
        } else {
            std::cout << "Error writing to index_file!" << std::endl;
        }


        database_file.flush();
        index_file.flush();
        
        if (index_file.good() && database_file.good()) {
            std::cout << "Inserted key and value into files." << std::endl;
        } else {
            std::cout << "Error writing to files!" << std::endl;
        }  
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

        // For every line in index_file
        while (std::getline(index_file, line)) {
            std::istringstream iss(line);

            if (iss >> temp_key >> comma >> offset) {
                if (temp_key == hashed_key) {
                    found = true;
                    line = "d" + std::to_string(temp_key) + SEPARATOR + std::to_string(offset);
                }
            }
            lines.push_back(line);
        }

        // Reopen the index file in write mode, truncating it
        index_file.close();  
        index_file.open("../database/index.idx", std::ios::out | std::ios::trunc);  

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
}
