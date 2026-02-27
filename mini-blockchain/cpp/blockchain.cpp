#include "blockchain.hpp"
#include "miner.hpp"

#include <ctime>

static uint64_t time_now();

Blockchain::Blockchain(uint32_t difficulty)
    : difficulty_(difficulty)
{
    chain_.push_back(create_genesis_block());
}

const Block& Blockchain::get_latest_block() const {
    return chain_.back();
}

void Blockchain::add_block(std::vector<Transaction> transactions) {
    const Block& prev = get_latest_block();
    Block b(prev.index() + 1, time_now(), std::move(transactions), prev.hash(), difficulty_);
    Miner::mine(b);
    chain_.push_back(std::move(b));
}

bool Blockchain::is_valid() const {
    for (std::size_t i = 1; i < chain_.size(); ++i) {
        const Block& cur  = chain_[i];
        const Block& prev = chain_[i - 1];

        if (cur.hash() != cur.compute_hash())
            return false;

        if (cur.hash().substr(0, cur.difficulty()) != std::string(cur.difficulty(), '0'))
            return false;

        if (cur.previous_hash() != prev.hash())
            return false;
    }
    return true;
}

// ---------------------------------------------------------------------------
// private helpers
// ---------------------------------------------------------------------------
Block Blockchain::create_genesis_block() {
    Block b(0, time_now(), {}, "0", difficulty_);
    Miner::mine(b);
    return b;
}

static uint64_t time_now() { return static_cast<uint64_t>(std::time(nullptr)); }
