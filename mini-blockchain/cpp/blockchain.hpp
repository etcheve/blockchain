#pragma once

#include "block.hpp"
#include "transaction.hpp"
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

class Blockchain {
public:
    explicit Blockchain(uint32_t difficulty = 4, double reward = 100.0);

    // Returns a candidate block ready to be mined: previous_hash, index,
    // miner, reward, and balances snapshot are all pre-filled.
    Block request_block(const std::string& miner, std::vector<Transaction> transactions);

    // Validates block against the current chain tip, then appends it.
    // Returns false (and does nothing) if the block is invalid.
    bool add_block(Block block);

    // Checks whether `block` would be a valid next block for this chain.
    bool is_valid(const Block& block) const;

    // Verifies the integrity of the entire stored chain.
    bool is_chain_valid() const;

    uint32_t   difficulty() const { return difficulty_; }
    double     reward()     const { return reward_; }

    const std::vector<Block>&                      chain()    const { return chain_; }
    const std::unordered_map<std::string, double>& balances() const { return chain_.back().balances(); }

private:
    std::vector<Block> chain_;
    uint32_t           difficulty_;
    double             reward_;

    const Block& get_latest_block() const;
    Block        create_genesis_block();
    std::unordered_map<std::string, double> compute_next_balances(
        const std::unordered_map<std::string, double>& prev,
        const Transaction& reward,
        const std::vector<Transaction>& transactions) const;
};
