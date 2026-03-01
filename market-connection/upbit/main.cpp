#include "connection/upbit_client.hpp"

#include <iostream>
#include <iomanip>

static void print_result(const std::string& label, const Response& r) {
    // market/all returns a large array — truncate for readability
    std::string body = r.body.size() > 200 ? r.body.substr(0, 200) + "..." : r.body;

    std::cout << label << "\n"
              << "  status:   " << r.http_code << "\n"
              << "  response: " << body << "\n"
              << "  latency:  " << std::fixed << std::setprecision(2)
              << r.elapsed_ms << " ms\n\n";
}

int main() {
    UpbitClient client;

    std::cout << "=== Upbit API (production public) ===\n"
              << "base: https://api.upbit.com/v1\n\n";

    print_result("market_list (truncated)", client.market_list());
    print_result("KRW-BTC",                client.ticker_price("KRW-BTC"));

    return 0;
}
