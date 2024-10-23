#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>
#include <limits>
#include <cstring>

#define SEPERATOR ", "

struct Hashmap {
    std::fstream index_file;
    std::fstream database_file;

    Hashmap(const std::string& db_path, const std::string& index_path) {
        database_file.open(db_path, std::ios::in | std::ios::out | std::ios::binary);
        index_file.open(index_path, std::ios::in | std::ios::out | std::ios::binary);

        if (!database_file.is_open() || !index_file.is_open()) {
            std::cerr << "Error opening files!" << std::endl;
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

    std::string get_val_from_key(const std::string& id_from_user) {
        uint64_t key = hashify(id_from_user);
    
        
        index_file.clear();
        index_file.seekg(0, std::ios::beg);
        
        uint64_t temp_key;
        long offset;
        char comma;
        
        
        while (index_file >> temp_key >> comma >> offset) {
            
            if (index_file.peek() == 'd') {
                index_file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }
            
            if (temp_key == key) {
                
                database_file.clear();
                database_file.seekg(offset, std::ios::beg);
                
                std::string line;
                std::getline(database_file, line);
                
                
                size_t sep_pos = line.find(SEPERATOR);
                if (sep_pos != std::string::npos) {
                    return line.substr(sep_pos + strlen(SEPERATOR));
                }
            }
            
            index_file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        
        std::cout << "Key not found: " << id_from_user << std::endl;
        return "";
    }

    void insert(uint64_t key, const std::string& value) {
        long offset = database_file.tellp();
        index_file << key << SEPERATOR << offset << std::endl;
        database_file << key << SEPERATOR << value << std::endl;
        std::cout << "Inserted key and value into files." << std::endl;
    }

    void remove(uint64_t key) {
        index_file.clear();
        index_file.seekg(0, std::ios::beg);

        uint64_t temp_key;
        long offset;
        char comma;
        bool found = false;

        std::string new_line;
        std::streampos line_start;

        while (index_file.good()) {
            line_start = index_file.tellg();

            if (index_file >> temp_key >> comma >> offset) {
                if (temp_key == key) {
                    found = true;
                    index_file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    new_line = "d" + std::to_string(temp_key) + SEPERATOR + std::to_string(offset) + "\n";
                } else {
                    std::string value;
                    std::getline(index_file, value, '\n');
                    new_line = std::to_string(temp_key) + SEPERATOR + std::to_string(offset) + "\n" + value;
                }

                index_file.seekp(line_start);
                index_file << new_line;
                index_file.flush();
            } else {
                break;
            }
        }

        if (found) {
            std::cout << "Marked key " << key << " as deleted." << std::endl;
        } else {
            std::cerr << "Key " << key << " not found." << std::endl;
        }
    }
};

int main() {
    Hashmap hashmap("../database/winkel.db", "../database/index.idx");
    
    return 0;
}