#pragma once

#include <string>

struct Response {
    std::string body;
    long        http_code;
    double      elapsed_ms;
};

class KrakenClient {
public:
    KrakenClient();
    ~KrakenClient();

    Response system_status();
    Response server_time();
    Response ticker_price(const std::string& pair);

private:
    static const std::string BASE_URL;

    Response get(const std::string& endpoint);
};
