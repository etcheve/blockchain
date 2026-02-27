#include "block.hpp"

#include <openssl/sha.h>
#include <iomanip>
#include <sstream>

static std::string bytes_to_hex(const unsigned char* data, std::size_t len);

Block::Block(uint64_t index, uint64_t timestamp,
             std::vector<Transaction> transactions,
             const std::string& previous_hash, uint32_t difficulty)
    : index_(index)
    , timestamp_(timestamp)
    , transactions_(std::move(transactions))
    , previous_hash_(previous_hash)
    , difficulty_(difficulty)
    , nonce_(0)
    , hash_(compute_hash())
{}

std::string Block::compute_hash() const {
    std::string payload =
        std::to_string(index_)      +
        std::to_string(timestamp_)  +
        serialize_transactions()    +
        previous_hash_              +
        std::to_string(difficulty_) +
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
std::string Block::serialize_transactions() const {
    std::string s;
    for (const auto& tx : transactions_)
        s += tx.from + tx.to + std::to_string(tx.amount);
    return s;
}

static std::string bytes_to_hex(const unsigned char* data, std::size_t len) {
    std::ostringstream oss;
    oss << std::hex << std::setfill('0');
    for (std::size_t i = 0; i < len; ++i)
        oss << std::setw(2) << static_cast<unsigned int>(data[i]);
    return oss.str();
}
