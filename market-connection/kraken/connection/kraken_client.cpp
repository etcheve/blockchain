#include "kraken_client.hpp"

#include <curl/curl.h>
#include <chrono>
#include <stdexcept>

const std::string KrakenClient::BASE_URL = "https://api.kraken.com/0/public";

// ── helpers ────────────────────────────────────────────────────────────────

static size_t write_callback(char* ptr, size_t size, size_t nmemb, std::string* out) {
    out->append(ptr, size * nmemb);
    return size * nmemb;
}

// ── public ─────────────────────────────────────────────────────────────────

KrakenClient::KrakenClient() {
    curl_global_init(CURL_GLOBAL_DEFAULT);
}

KrakenClient::~KrakenClient() {
    curl_global_cleanup();
}

Response KrakenClient::system_status() {
    return get("/SystemStatus");
}

Response KrakenClient::server_time() {
    return get("/Time");
}

Response KrakenClient::ticker_price(const std::string& pair) {
    return get("/Ticker?pair=" + pair);
}

// ── private ────────────────────────────────────────────────────────────────

Response KrakenClient::get(const std::string& endpoint) {
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
