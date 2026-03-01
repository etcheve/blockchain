#pragma once

#include <string>

struct Response {
    std::string body;
    long        http_code;
    double      elapsed_ms;
};

class UpbitClient {
public:
    UpbitClient();
    ~UpbitClient();

    Response market_list();
    Response ticker_price(const std::string& market);

private:
    static const std::string BASE_URL;

    Response get(const std::string& endpoint);
};
