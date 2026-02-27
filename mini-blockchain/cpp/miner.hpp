#pragma once

#include "block.hpp"

class Miner {
public:
    // Increment block's nonce until its hash meets its own difficulty target.
    // Writes nonce, hash, and mine_time_ms directly into the block.
    static void mine(Block& block);
};
