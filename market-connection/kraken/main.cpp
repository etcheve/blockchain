#include "connection/kraken_client.hpp"

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
    KrakenClient client;

    std::cout << "=== Kraken Spot API (production public) ===\n"
              << "base: https://api.kraken.com/0/public\n\n";

    print_result("system_status", client.system_status());
    print_result("server_time",   client.server_time());
    print_result("XBT/USD",       client.ticker_price("XBTUSD"));

    return 0;
}
