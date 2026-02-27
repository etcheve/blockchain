#include "blockchain.hpp"
#include <iostream>
#include <iomanip>

static void print_block(const Block& b) {
    std::cout << "  index         : " << b.index()         << "\n"
              << "  timestamp     : " << b.timestamp()     << "\n"
              << "  previous_hash : " << b.previous_hash() << "\n"
              << "  difficulty    : " << b.difficulty()    << "\n"
              << "  nonce         : " << b.nonce()         << "\n"
              << "  hash          : " << b.hash()          << "\n";

    std::cout << "  transactions  :\n";
    for (const auto& tx : b.transactions())
        std::cout << "    " << tx.from << " -> " << tx.to
                  << " : " << std::fixed << std::setprecision(2) << tx.amount << "\n";
}

int main() {
    const uint32_t DIFFICULTY = 4;
    Blockchain bc(DIFFICULTY);

    bc.add_block({
        {"Alice", "Bob",     50.0},
        {"Bob",   "Charlie", 20.0},
    });
    bc.add_block({
        {"Charlie", "Alice", 15.0},
    });
    bc.add_block({
        {"Alice", "Dave",  100.0},
        {"Dave",  "Eve",    30.0},
        {"Eve",   "Alice",  10.0},
    });

    std::cout << "=== Blockchain (difficulty=" << bc.difficulty() << ") ===\n\n";
    for (const auto& block : bc.chain()) {
        std::cout << "Block #" << block.index() << "\n";
        print_block(block);
        std::cout << "\n";
    }

    std::cout << "Chain valid: " << (bc.is_valid() ? "true" : "false") << "\n";
    return 0;
}
