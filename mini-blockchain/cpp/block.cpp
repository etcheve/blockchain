#include "block.hpp"

#include <openssl/sha.h>
#include <iomanip>
#include <sstream>


// ---------------------------------------------------------------------------
// Block
// ---------------------------------------------------------------------------
Block::Block(uint64_t index, uint64_t timestamp, const std::string& data,
             const std::string& previous_hash)
    : index_(index)
    , timestamp_(timestamp)
    , data_(data)
    , previous_hash_(previous_hash)
    , hash_(compute_hash())   // computed once, stored, never mutated
{}

std::string Block::compute_hash() const {
    // Concatenate: index + timestamp + data + previous_hash (all as strings)
    std::string payload =
        std::to_string(index_) +
        std::to_string(timestamp_) +
        data_ +
        previous_hash_;

    unsigned char digest[SHA256_DIGEST_LENGTH];
    SHA256(reinterpret_cast<const unsigned char*>(payload.data()),
           payload.size(),
           digest);

    return bytes_to_hex(digest, SHA256_DIGEST_LENGTH);
}

// ---------------------------------------------------------------------------
// Internal helper: SHA256 bytes -> lowercase hex string
// ---------------------------------------------------------------------------
static std::string bytes_to_hex(const unsigned char* data, std::size_t len) {
    std::ostringstream oss;
    oss << std::hex << std::setfill('0');
    for (std::size_t i = 0; i < len; ++i)
        oss << std::setw(2) << static_cast<unsigned int>(data[i]);
    return oss.str();
}
