#include "connection/coinbase_client.hpp"

#include <iostream>
#include <iomanip>

static void print_result(const std::string& label, const Response& r) {
    std::cout << label << "\n"
              << "  status:   " << r.http_code << "\n"
              << "  response: " << r.body << "\n"
              << "  latency:  " << std::fixed << std::setprecision(2)
              << r.elapsed_ms << " ms\n\n";
}

int main() {
    CoinbaseClient client;

    std::cout << "=== Coinbase Exchange Sandbox ===\n"
              << "base: https://api-public.sandbox.exchange.coinbase.com\n\n";

    print_result("server_time", client.server_time());
    print_result("BTC-USD",     client.ticker_price("BTC-USD"));

    return 0;
}
