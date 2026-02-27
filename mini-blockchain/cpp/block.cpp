#include "block.hpp"

#include <openssl/sha.h>
#include <algorithm>
#include <iomanip>
#include <sstream>

static std::string bytes_to_hex(const unsigned char* data, std::size_t len);

Block::Block(uint64_t index, uint64_t timestamp,
             std::vector<Transaction> transactions,
             const std::string& previous_hash,
             const std::string& miner,
             Transaction reward,
             std::unordered_map<std::string, double> balances,
             uint32_t difficulty)
    : index_(index)
    , timestamp_(timestamp)
    , transactions_(std::move(transactions))
    , previous_hash_(previous_hash)
    , miner_(miner)
    , reward_(std::move(reward))
    , balances_(std::move(balances))
    , difficulty_(difficulty)
    , nonce_(0)
    , hash_(compute_hash())
{}

std::string Block::compute_hash() const {
    std::string payload =
        std::to_string(index_)         +
        std::to_string(timestamp_)     +
        serialize_transactions()       +
        previous_hash_                 +
        miner_                         +
        serialize_transaction(reward_) +
        serialize_balances()           +
        std::to_string(difficulty_)    +
        std::to_string(nonce_);

    unsigned char digest[SHA256_DIGEST_LENGTH];
    SHA256(reinterpret_cast<const unsigned char*>(payload.data()),
           payload.size(),
           digest);

    return bytes_to_hex(digest, SHA256_DIGEST_LENGTH);
}

// ---------------------------------------------------------------------------
// private helpers
// ---------------------------------------------------------------------------
std::string Block::serialize_transaction(const Transaction& tx) const {
    return tx.from + tx.to + std::to_string(tx.amount);
}

std::string Block::serialize_transactions() const {
    std::string s;
    for (const auto& tx : transactions_)
        s += serialize_transaction(tx);
    return s;
}

std::string Block::serialize_balances() const {
    std::vector<std::pair<std::string, double>> sorted(balances_.begin(), balances_.end());
    std::sort(sorted.begin(), sorted.end());
    std::string s;
    for (const auto& [key, val] : sorted)
        s += key + std::to_string(val);
    return s;
}

static std::string bytes_to_hex(const unsigned char* data, std::size_t len) {
    std::ostringstream oss;
    oss << std::hex << std::setfill('0');
    for (std::size_t i = 0; i < len; ++i)
        oss << std::setw(2) << static_cast<unsigned int>(data[i]);
    return oss.str();
}
