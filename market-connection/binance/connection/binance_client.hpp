#pragma once

#include <string>

struct Response {
    std::string body;
    long        http_code;
    double      elapsed_ms;
};

class BinanceClient {
public:
    BinanceClient();
    ~BinanceClient();

    Response ping();
    Response server_time();
    Response ticker_price(const std::string& symbol);

private:
    static const std::string BASE_URL;

    Response get(const std::string& endpoint);
};
