#include "blockchain.hpp"
#include "miner.hpp"
#include <algorithm>
#include <iostream>
#include <iomanip>

static void print_block(const Block& b) {
    std::cout << "  index         : " << b.index()         << "\n"
              << "  timestamp     : " << b.timestamp()     << "\n"
              << "  previous_hash : " << b.previous_hash() << "\n"
              << "  miner         : " << b.miner()         << "\n"
              << "  difficulty    : " << b.difficulty()    << "\n"
              << "  nonce         : " << b.nonce()         << "\n"
              << "  hash          : " << b.hash()          << "\n";

    const auto& r = b.reward();
    std::cout << "  reward        : " << r.from << " -> " << r.to
              << " : " << std::fixed << std::setprecision(2) << r.amount << "\n";

    std::cout << "  transactions  :\n";
    for (const auto& tx : b.transactions())
        std::cout << "    " << tx.from << " -> " << tx.to
                  << " : " << std::fixed << std::setprecision(2) << tx.amount << "\n";

    std::vector<std::pair<std::string, double>> sorted_bal(b.balances().begin(), b.balances().end());
    std::sort(sorted_bal.begin(), sorted_bal.end());
    std::cout << "  balances      :\n";
    for (const auto& [user, bal] : sorted_bal)
        std::cout << "    " << user << " : " << std::fixed << std::setprecision(2) << bal << "\n";
}

int main() {
    Blockchain bc(4, 100.0);

    // Each miner requests a candidate block, mines it, then submits it.
    {
        auto block = bc.request_block("Alice", {
            {"Alice", "Bob",     50.0},
            {"Bob",   "Charlie", 20.0},
        });
        Miner::mine(block);
        bc.add_block(std::move(block));
    }
    {
        auto block = bc.request_block("Bob", {
            {"Charlie", "Alice", 15.0},
        });
        Miner::mine(block);
        bc.add_block(std::move(block));
    }
    {
        auto block = bc.request_block("Charlie", {
            {"Alice", "Dave",  100.0},
            {"Dave",  "Eve",    30.0},
            {"Eve",   "Alice",  10.0},
        });
        Miner::mine(block);
        bc.add_block(std::move(block));
    }

    std::cout << "=== Blockchain (difficulty=" << bc.difficulty()
              << ", reward=" << std::fixed << std::setprecision(2) << bc.reward() << ") ===\n\n";
    for (const auto& block : bc.chain()) {
        std::cout << "Block #" << block.index() << "\n";
        print_block(block);
        std::cout << "\n";
    }

    std::cout << "Chain valid: " << (bc.is_chain_valid() ? "true" : "false") << "\n";
    return 0;
}
