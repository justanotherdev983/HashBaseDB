#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdint>

namespace Hash {
    uint64_t hashify(const std::string& value);
    size_t get_bucket(uint64_t hashed_key, size_t num_buckets);
    int64_t calculate_bucket_position(size_t bucket, size_t bucket_size);
    void debug_hash(const std::string& key);
}