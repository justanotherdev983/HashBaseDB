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

    size_t get_bucket(uint64_t hashed_key, size_t num_buckets) {
        return static_cast<size_t>(hashed_key % num_buckets);
    }
    
    int64_t calculate_bucket_position(size_t bucket, size_t bucket_size, size_t num_buckets) {
        if (bucket >= num_buckets) {
            throw std::out_of_range("Bucket index exceeds maximum number of buckets");
        }
        
        // Calculate byte offset
        int64_t position = static_cast<int64_t>(bucket) * static_cast<int64_t>(bucket_size);
        
        // Validate the calculated position
        if (position < 0 || position >= static_cast<int64_t>(num_buckets * bucket_size)) {
            throw std::out_of_range("Calculated bucket position is out of valid range");
        }
        
        return position;
    }

    void debug_hash(const std::string& key) {
        uint64_t hash = hashify(key);
        size_t bucket = get_bucket(hash, 1000);
        int64_t bucket_position = calculate_bucket_position(bucket, 64, 1000);
        
        std::cout << "Key: " << key << "\n";
        std::cout << "Hash: " << hash << "\n";
        std::cout << "Bucket: " << bucket << "\n";
        std::cout << "Bucket position: " << bucket_position << "\n";
    }

}