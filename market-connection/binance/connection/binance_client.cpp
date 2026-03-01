#include "binance_client.hpp"

#include <curl/curl.h>
#include <stdexcept>

const std::string BinanceClient::BASE_URL = "https://testnet.binance.vision/api/v3";

// ── helpers ────────────────────────────────────────────────────────────────

static size_t write_callback(char* ptr, size_t size, size_t nmemb, std::string* out) {
    out->append(ptr, size * nmemb);
    return size * nmemb;
}

// ── public ─────────────────────────────────────────────────────────────────

BinanceClient::BinanceClient() {
    curl_global_init(CURL_GLOBAL_DEFAULT);
}

BinanceClient::~BinanceClient() {
    curl_global_cleanup();
}

Response BinanceClient::ping() {
    return get("/ping");
}

Response BinanceClient::server_time() {
    return get("/time");
}

Response BinanceClient::ticker_price(const std::string& symbol) {
    return get("/ticker/price?symbol=" + symbol);
}

// ── private ────────────────────────────────────────────────────────────────

Response BinanceClient::get(const std::string& endpoint) {
    CURL* curl = curl_easy_init();
    if (!curl)
        throw std::runtime_error("Failed to init curl");

    Response res{};
    std::string url = BASE_URL + endpoint;

    curl_easy_setopt(curl, CURLOPT_URL,           url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA,     &res.body);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT,       10L);

    auto t0 = std::chrono::high_resolution_clock::now();

    CURLcode code = curl_easy_perform(curl);

    auto t1 = std::chrono::high_resolution_clock::now();
    res.elapsed_ms = std::chrono::duration<double, std::milli>(t1 - t0).count();

    if (code != CURLE_OK) {
        curl_easy_cleanup(curl);
        throw std::runtime_error(curl_easy_strerror(code));
    }

    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &res.http_code);
    curl_easy_cleanup(curl);
    return res;
}
