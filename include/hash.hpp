#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdint>

namespace Hash {
    uint64_t hashify(const std::string& value);
    uint64_t get_bucket(uint64_t hashed_key, size_t num_buckets);
}