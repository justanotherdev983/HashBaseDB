#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <stdexcept>
#include <filesystem>
#include <fstream>
#include <cstring>

#include "core/hash.hpp"

namespace Hash {

    uint64_t hashify(const std::string& value) {
        // if (value.empty()) {
        //     throw HashingError("Cannot hash empty string");
        // }

        const uint64_t FNV_prime = 0x100000001b3;
        uint64_t hash_value = 0xcbf29ce484222325;

        try {
            for (char c : value) {
                hash_value ^= static_cast<uint64_t>(c);
                if (hash_value == 0) {
                    throw HashingError("Hash collision detected - zero hash value");
                }
                hash_value *= FNV_prime;
            }
        } catch (const std::exception& e) {
            throw HashingError(std::string("Hashing failed: ") + e.what());
        }

        return hash_value;
    }

    size_t get_bucket(uint64_t hashed_key, size_t num_buckets) {
        if (num_buckets == 0) {
            throw BucketError("Number of buckets cannot be zero");
        }
        return static_cast<size_t>(hashed_key % num_buckets);
    }
    
    int64_t calculate_bucket_position(size_t bucket, size_t bucket_size, size_t num_buckets) {
        if (bucket >= num_buckets) {
            throw BucketError("Bucket index " + std::to_string(bucket) + 
                            " exceeds maximum number of buckets " + std::to_string(num_buckets));
        }
        
        if (bucket_size == 0) {
            throw BucketError("Bucket size cannot be zero");
        }

        // Check for potential overflow before multiplication
        if (bucket > std::numeric_limits<int64_t>::max() / static_cast<int64_t>(bucket_size)) {
            throw BucketError("Bucket position calculation would overflow");
        }
        
        int64_t position = static_cast<int64_t>(bucket) * static_cast<int64_t>(bucket_size);
        
        // Validate the calculated position
        if (position < 0) {
            throw BucketError("Calculated negative bucket position");
        }

        int64_t max_position = static_cast<int64_t>(num_buckets) * static_cast<int64_t>(bucket_size);
        if (position >= max_position) {
            throw BucketError("Calculated bucket position exceeds maximum allowed position");
        }
        
        return position;
    }

    void debug_hash(const std::string& key, size_t num_buckets, size_t bucket_size) {
        try {
            uint64_t hash = hashify(key);
            size_t bucket = get_bucket(hash, num_buckets);
            int64_t bucket_position = calculate_bucket_position(bucket, bucket_size, num_buckets);
            
            std::cout << "Debug Hash Information:\n"
                      << "Key: " << key << "\n"
                      << "Hash: " << hash << "\n"
                      << "Bucket: " << bucket << "\n"
                      << "Bucket position: " << bucket_position << "\n";
        } catch (const std::exception& e) {
            std::cerr << "Debug hash failed: " << e.what() << std::endl;
        }
    }
}