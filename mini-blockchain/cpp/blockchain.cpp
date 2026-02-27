#include "blockchain.hpp"
#include "miner.hpp"

#include <ctime>


// ---------------------------------------------------------------------------
// helpers
// ---------------------------------------------------------------------------
static uint64_t time_now() {
    return static_cast<uint64_t>(std::time(nullptr));
}


// ---------------------------------------------------------------------------
// Blockchain
// ---------------------------------------------------------------------------
Blockchain::Blockchain(uint32_t difficulty)
    : difficulty_(difficulty)
{
    chain_.push_back(create_genesis_block());
}

Block Blockchain::create_genesis_block() {
    Block b(0, time_now(), "Genesis Block", "0", difficulty_);
    Miner::mine(b);
    return b;
}

const Block& Blockchain::get_latest_block() const {
    return chain_.back();
}

void Blockchain::add_block(const std::string& data) {
    const Block& prev = get_latest_block();
    Block b(prev.index() + 1, time_now(), data, prev.hash(), difficulty_);
    Miner::mine(b);
    chain_.push_back(std::move(b));
}

bool Blockchain::is_valid() const {
    for (std::size_t i = 1; i < chain_.size(); ++i) {
        const Block& cur  = chain_[i];
        const Block& prev = chain_[i - 1];

        // Hash integrity: stored hash must match a fresh recomputation
        if (cur.hash() != cur.compute_hash())
            return false;

        // Proof of work: hash must meet the block's own difficulty target
        if (cur.hash().substr(0, cur.difficulty()) != std::string(cur.difficulty(), '0'))
            return false;

        // Chain linkage: this block must point at the previous block's hash
        if (cur.previous_hash() != prev.hash())
            return false;
    }
    return true;
}
