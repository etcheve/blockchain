#include "connection/bybit_client.hpp"

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
    BybitClient client;

    std::cout << "=== Bybit Testnet ===\n"
              << "base: https://api-testnet.bybit.com\n\n";

    print_result("server_time", client.server_time());
    print_result("BTC/USDT",    client.ticker_price("BTCUSDT"));

    return 0;
}
