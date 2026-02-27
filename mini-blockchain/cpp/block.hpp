#pragma once

#include "transaction.hpp"
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

class Block {
public:
    Block(uint64_t index, uint64_t timestamp,
          std::vector<Transaction> transactions,
          const std::string& previous_hash,
          const std::string& miner,
          Transaction reward,
          std::unordered_map<std::string, double> balances,
          uint32_t difficulty);

    uint64_t                                      index()        const { return index_; }
    uint64_t                                      timestamp()    const { return timestamp_; }
    const std::vector<Transaction>&               transactions() const { return transactions_; }
    std::string                                   previous_hash() const { return previous_hash_; }
    std::string                                   miner()        const { return miner_; }
    const Transaction&                            reward()       const { return reward_; }
    const std::unordered_map<std::string, double>& balances()    const { return balances_; }
    std::string                                   hash()         const { return hash_; }
    uint64_t                                      nonce()        const { return nonce_; }
    uint32_t                                      difficulty()   const { return difficulty_; }

    void set_nonce(uint64_t n)          { nonce_ = n; }
    void set_hash(const std::string& h) { hash_  = h; }

    std::string compute_hash() const;

private:
    uint64_t                                 index_;
    uint64_t                                 timestamp_;
    std::vector<Transaction>                 transactions_;
    std::string                              previous_hash_;
    std::string                              miner_;
    Transaction                              reward_;
    std::unordered_map<std::string, double>  balances_;
    uint32_t                                 difficulty_ = 0;
    uint64_t                                 nonce_      = 0;
    std::string                              hash_;

    std::string serialize_transaction(const Transaction& tx) const;
    std::string serialize_transactions() const;
    std::string serialize_balances() const;
};
