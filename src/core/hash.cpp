#include <iostream>

namespace Hash {
    uint64_t hashify(const std::string& value) {
        const uint64_t FNV_prime = 0x100000001b3;
        uint64_t hash_value = 0xcbf29ce484222325;

        for (char c : value) {
            hash_value ^= c;
            hash_value *= FNV_prime;
        }
        return hash_value;
    }

    uint64_t get_bucket(uint64_t hashed_key, size_t num_buckets) {
        return hashed_key % num_buckets;
    }

}