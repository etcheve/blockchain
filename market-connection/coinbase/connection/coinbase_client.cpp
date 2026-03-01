#include "coinbase_client.hpp"

#include <curl/curl.h>
#include <chrono>
#include <stdexcept>

const std::string CoinbaseClient::BASE_URL = "https://api-public.sandbox.exchange.coinbase.com";

// ── helpers ────────────────────────────────────────────────────────────────

static size_t write_callback(char* ptr, size_t size, size_t nmemb, std::string* out) {
    out->append(ptr, size * nmemb);
    return size * nmemb;
}

// ── public ─────────────────────────────────────────────────────────────────

CoinbaseClient::CoinbaseClient() {
    curl_global_init(CURL_GLOBAL_DEFAULT);
}

CoinbaseClient::~CoinbaseClient() {
    curl_global_cleanup();
}

Response CoinbaseClient::server_time() {
    return get("/time");
}

Response CoinbaseClient::ticker_price(const std::string& product) {
    return get("/products/" + product + "/ticker");
}

// ── private ────────────────────────────────────────────────────────────────

Response CoinbaseClient::get(const std::string& endpoint) {
    CURL* curl = curl_easy_init();
    if (!curl)
        throw std::runtime_error("Failed to init curl");

    Response res{};
    std::string url = BASE_URL + endpoint;

    curl_easy_setopt(curl, CURLOPT_URL,           url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA,     &res.body);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT,       10L);
    curl_easy_setopt(curl, CURLOPT_USERAGENT,     "market-connection/1.0");

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
