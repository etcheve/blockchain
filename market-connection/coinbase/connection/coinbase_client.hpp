#pragma once

#include <string>

struct Response {
    std::string body;
    long        http_code;
    double      elapsed_ms;
};

class CoinbaseClient {
public:
    CoinbaseClient();
    ~CoinbaseClient();

    Response server_time();
    Response ticker_price(const std::string& product);

private:
    static const std::string BASE_URL;

    Response get(const std::string& endpoint);
};
