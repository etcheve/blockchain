#pragma once

#include "block.hpp"
#include <vector>
#include <string>

class Blockchain {
public:
    Blockchain();                         // creates the genesis block

    const Block& get_latest_block() const;
    void         add_block(const std::string& data);
    bool         is_valid() const;

    // Read-only access to the full chain (useful for printing / testing)
    const std::vector<Block>& chain() const { return chain_; }

private:
    std::vector<Block> chain_;

    static Block create_genesis_block();
};
