#pragma once

#include <cstdint>
#include <string>

class Block {
public:
    Block(uint64_t index, uint64_t timestamp, const std::string& data,
          const std::string& previous_hash, uint32_t difficulty);

    uint64_t    index()         const { return index_; }
    uint64_t    timestamp()     const { return timestamp_; }
    std::string data()          const { return data_; }
    std::string previous_hash() const { return previous_hash_; }
    std::string hash()          const { return hash_; }
    uint64_t    nonce()         const { return nonce_; }
    uint32_t    difficulty()    const { return difficulty_; }

    void set_nonce(uint64_t n) { nonce_ = n; }
    void set_hash(const std::string& h) { hash_ = h; }

    // Recompute SHA256(index || timestamp || data || previous_hash || difficulty || nonce).
    // Used during validation — does NOT mutate the block.
    std::string compute_hash() const;

private:
    uint64_t    index_;
    uint64_t    timestamp_;
    std::string data_;
    std::string previous_hash_;
    uint32_t    difficulty_   = 0;
    uint64_t    nonce_        = 0;
    std::string hash_;
};
