#pragma once

#include "block.hpp"
#include <cstdint>
#include <vector>
#include <string>

class Blockchain {
public:
    explicit Blockchain(uint32_t difficulty = 4);   // creates the genesis block

    const Block& get_latest_block() const;
    void         add_block(const std::string& data);
    bool         is_valid() const;

    uint32_t difficulty() const { return difficulty_; }

    // Read-only access to the full chain (useful for printing / testing)
    const std::vector<Block>& chain() const { return chain_; }

private:
    std::vector<Block> chain_;
    uint32_t           difficulty_;

    Block create_genesis_block();
};
