#include "connection/binance_client.hpp"

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
    BinanceClient client;

    std::cout << "=== Binance Spot Testnet ===\n"
              << "base: https://testnet.binance.vision/api/v3\n\n";

    print_result("ping",        client.ping());
    print_result("server_time", client.server_time());
    print_result("BTC/USDT",    client.ticker_price("BTCUSDT"));

    return 0;
}
