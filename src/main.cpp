#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>
#include <limits>
#include <cstring>
#include <vector>
#include <sstream>

#define SEPERATOR ", "
#define NUM_BUCKETS 100
#define BUCKET_SIZE 64

struct Hashmap {
    std::fstream index_file;
    std::fstream database_file;

    Hashmap(const std::string& db_path, const std::string& index_path) {
        database_file.open(db_path, std::ios::in | std::ios::out | std::ios::app);
        index_file.open(index_path, std::ios::in | std::ios::out | std::ios::app);

        if (!database_file.is_open() || !index_file.is_open()) {
            std::cout << "Error opening files!" << std::endl;
        } else {
            std::cout << "Files opened successfully." << std::endl;
        }
    }

    ~Hashmap() {
        if (database_file.is_open()) {
            database_file.close();
            std::cout << "Database file closed." << std::endl;
        }
        if (index_file.is_open()) {
            index_file.close();
            std::cout << "Index file closed." << std::endl;
        }
    }

    uint64_t hashify(const std::string& value) {
        const uint64_t FNV_prime = 0x100000001b3;
        uint64_t hash_value = 0xcbf29ce484222325;

        for (char c : value) {
            hash_value ^= c;
            hash_value *= FNV_prime;
        }
        return hash_value;
    }

    std::vector<std::string> get_val_from_key(const std::string& id_from_user) {
        uint64_t key = hashify(id_from_user);
        std::vector<std::string> values;
        uint64_t temp_key;
        long offset;
        char comma;
        uint64_t bucket = key % NUM_BUCKETS;
        
        index_file.clear();
        index_file.seekg(bucket * BUCKET_SIZE, std::ios::beg);
        
        while (index_file >> temp_key >> comma >> offset) {
            
            if (index_file.peek() == 'd') {
                index_file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Encountered deleted key" << std::endl;
                continue;
            }
            
            if (temp_key == key) {
                
                
                database_file.clear();
                database_file.seekg(offset, std::ios::beg);
                
                std::string line;
                std::getline(database_file, line);
                
                std::istringstream iss(line); // size of seperator
                iss >> offset;
                
                
                size_t sep_pos = line.find(SEPERATOR);
                if (sep_pos != std::string::npos) {
                    values.push_back(line.substr(sep_pos + strlen(SEPERATOR)));
                }
            }
            
            index_file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        return values;
    }

    void insert(uint64_t key, const std::vector<std::string>& values) {
        database_file.clear();
        index_file.clear();

        std::cout << "Starting insertion \n" << "Key is: "<< key << std::endl;
        database_file.seekp(0, std::ios::end);
        long offset = database_file.tellp();

        std::stringstream line;

        
        line << key << SEPERATOR;

        for (int i = 0; i < values.size(); ++i) {
            line << values[i];
            std::cout << values[i] << std::endl;
            
            if (i < values.size() - 1) {
                line << SEPERATOR;
            }
        }

        line << "\n";
        std::cout << line.str() << std::endl;

        if (database_file << line.str()) {
            std::cout << "Inserted key and value into database succesfully" << std::endl;
        } else {
            std::cout << "Error writing to database_file!" << std::endl;
        }

        uint64_t bucket = key % NUM_BUCKETS;
        index_file.seekp(bucket * BUCKET_SIZE, std::ios::beg); // Find the correct bucket
        
        if (index_file << key << SEPERATOR << offset << std::endl) {
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

    void remove(uint64_t key) {
        index_file.clear();
        index_file.seekg(0, std::ios::beg);

        uint64_t temp_key;
        long offset;
        char comma;

        
        std::vector<std::string> lines;
        std::string line;
        bool found = false;

        // For every line in index_file
        while (std::getline(index_file, line)) {
            std::istringstream iss(line);

            if (iss >> temp_key >> comma >> offset) {
                if (temp_key == key) {
                    found = true;
                    line = "d" + std::to_string(temp_key) + SEPERATOR + std::to_string(offset);
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
};

int main() {
    Hashmap hashmap("../database/winkel.db", "../database/index.idx");
    hashmap.debug_print_files();
    
    return 0;
}