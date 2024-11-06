#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdint>

namespace Hash {

    class HashingError : public std::runtime_error {
        public:
            explicit HashingError(const std::string& msg) : std::runtime_error(msg) {}
    };

    class BucketError : public std::runtime_error {
        public:
            explicit BucketError(const std::string& msg) : std::runtime_error(msg) {}
    };

    uint64_t hashify(const std::string& value);
    size_t get_bucket(uint64_t hashed_key, size_t num_buckets);
    int64_t calculate_bucket_position(size_t bucket, size_t bucket_size, size_t num_buckets);
    void debug_hash(const std::string& key, size_t num_buckets, size_t bucket_size);
}