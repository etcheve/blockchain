#include "blockchain.hpp"
#include <iostream>

static void print_block(const Block& b) {
    std::cout << "  index         : " << b.index()         << "\n"
              << "  timestamp     : " << b.timestamp()     << "\n"
              << "  data          : " << b.data()          << "\n"
              << "  previous_hash : " << b.previous_hash() << "\n"
              << "  hash          : " << b.hash()          << "\n";
}

int main() {
    Blockchain bc;

    bc.add_block("Block 1 - hello blockchain");
    bc.add_block("Block 2 - another transaction");
    bc.add_block("Block 3 - yet more data");

    std::cout << "=== Blockchain ===\n";
    for (const auto& block : bc.chain()) {
        std::cout << "Block #" << block.index() << "\n";
        print_block(block);
        std::cout << "\n";
    }

    std::cout << "Chain valid: " << (bc.is_valid() ? "true" : "false") << "\n";
    return 0;
}
