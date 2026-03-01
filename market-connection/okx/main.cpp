#include "connection/okx_client.hpp"

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
    OkxClient client;

    std::cout << "=== OKX (simulated trading / sandbox) ===\n"
              << "base: https://www.okx.com\n"
              << "mode: x-simulated-trading: 1\n\n";

    print_result("server_time", client.server_time());
    print_result("BTC-USDT",    client.ticker_price("BTC-USDT"));

    return 0;
}
