#include "blockchain.hpp"
#include "miner.hpp"

#include <ctime>
#include <iostream>

static uint64_t time_now();

Blockchain::Blockchain(uint32_t difficulty, double reward)
    : difficulty_(difficulty)
    , reward_(reward)
{
    chain_.push_back(create_genesis_block());
}

const Block& Blockchain::get_latest_block() const {
    return chain_.back();
}

Block Blockchain::request_block(const std::string& miner, std::vector<Transaction> transactions) {
    const Block& prev     = get_latest_block();
    Transaction  reward_tx{ "network", miner, reward_ };
    auto         balances = compute_next_balances(prev.balances(), reward_tx, transactions);

    return Block(prev.index() + 1, time_now(),
                 std::move(transactions), prev.hash(),
                 miner, std::move(reward_tx),
                 std::move(balances), difficulty_);
}

bool Blockchain::add_block(Block block) {
    if (!is_valid(block)) {
        std::cerr << "[Blockchain] rejected block #" << block.index() << " from " << block.miner() << "\n";
        return false;
    }
    chain_.push_back(std::move(block));
    return true;
}

bool Blockchain::is_valid(const Block& block) const {
    const Block& tip = get_latest_block();

    if (block.index()         != tip.index() + 1)       return false;
    if (block.previous_hash() != tip.hash())             return false;
    if (block.difficulty()    != difficulty_)            return false;
    if (block.hash()          != block.compute_hash())   return false;
    if (block.hash().substr(0, difficulty_) != std::string(difficulty_, '0')) return false;

    return true;
}

bool Blockchain::is_chain_valid() const {
    for (std::size_t i = 1; i < chain_.size(); ++i) {
        const Block& cur  = chain_[i];
        const Block& prev = chain_[i - 1];

        if (cur.hash()          != cur.compute_hash())                          return false;
        if (cur.previous_hash() != prev.hash())                                 return false;
        if (cur.hash().substr(0, difficulty_) != std::string(difficulty_, '0')) return false;
    }
    return true;
}

// ---------------------------------------------------------------------------
// private helpers
// ---------------------------------------------------------------------------
Block Blockchain::create_genesis_block() {
    Transaction reward_tx{ "network", "system", reward_ };
    auto        balances = compute_next_balances({}, reward_tx, {});
    Block b(0, time_now(), {}, "0", "system", std::move(reward_tx), std::move(balances), difficulty_);
    Miner::mine(b);
    return b;
}

std::unordered_map<std::string, double> Blockchain::compute_next_balances(
    const std::unordered_map<std::string, double>& prev,
    const Transaction& reward,
    const std::vector<Transaction>& transactions) const
{
    auto bal = prev;
    bal[reward.to] += reward.amount;
    for (const auto& tx : transactions) {
        bal[tx.from] -= tx.amount;
        bal[tx.to]   += tx.amount;
    }
    return bal;
}

static uint64_t time_now() { return static_cast<uint64_t>(std::time(nullptr)); }
