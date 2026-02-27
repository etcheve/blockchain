#pragma once

#include <cstdint>
#include <string>

class Block {
public:
    Block(uint64_t index, uint64_t timestamp, const std::string& data,
          const std::string& previous_hash);

    uint64_t    index()         const { return index_; }
    uint64_t    timestamp()     const { return timestamp_; }
    std::string data()          const { return data_; }
    std::string previous_hash() const { return previous_hash_; }
    std::string hash()          const { return hash_; }

    // Recompute SHA256(index || timestamp || data || previous_hash) and return it.
    // Used during validation, does NOT mutate the block.
    std::string compute_hash() const;

private:
    uint64_t    index_;
    uint64_t    timestamp_;
    std::string data_;
    std::string previous_hash_;
    std::string hash_;   // set once in the constructor, never changed
};
