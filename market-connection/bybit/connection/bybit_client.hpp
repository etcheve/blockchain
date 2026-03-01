#pragma once

#include <string>

struct Response {
    std::string body;
    long        http_code;
    double      elapsed_ms;
};

class BybitClient {
public:
    BybitClient();
    ~BybitClient();

    Response server_time();
    Response ticker_price(const std::string& symbol);

private:
    static const std::string BASE_URL;

    Response get(const std::string& endpoint);
};
