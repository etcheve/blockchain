#include "miner.hpp"

#include <chrono>
#include <cstdio>

void Miner::mine(Block& block) {
    std::string target(block.difficulty(), '0');

    auto t0 = std::chrono::high_resolution_clock::now();

    block.set_nonce(0);
    block.set_hash(block.compute_hash());
    while (block.hash().substr(0, block.difficulty()) != target) {
        block.set_nonce(block.nonce() + 1);
        block.set_hash(block.compute_hash());
    }

    auto t1      = std::chrono::high_resolution_clock::now();
    double ms    = std::chrono::duration<double, std::milli>(t1 - t0).count();
    std::printf("[Miner] block #%llu mined in %.2f ms (nonce=%llu)\n",
                static_cast<unsigned long long>(block.index()),
                ms,
                static_cast<unsigned long long>(block.nonce()));
}
