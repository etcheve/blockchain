#include "okx_client.hpp"

#include <curl/curl.h>
#include <chrono>
#include <stdexcept>

const std::string OkxClient::BASE_URL = "https://www.okx.com";

// ── helpers ────────────────────────────────────────────────────────────────

static size_t write_callback(char* ptr, size_t size, size_t nmemb, std::string* out) {
    out->append(ptr, size * nmemb);
    return size * nmemb;
}

// ── public ─────────────────────────────────────────────────────────────────

OkxClient::OkxClient() {
    curl_global_init(CURL_GLOBAL_DEFAULT);
}

OkxClient::~OkxClient() {
    curl_global_cleanup();
}

Response OkxClient::server_time() {
    return get("/api/v5/public/time");
}

Response OkxClient::ticker_price(const std::string& inst_id) {
    return get("/api/v5/market/ticker?instId=" + inst_id);
}

// ── private ────────────────────────────────────────────────────────────────

Response OkxClient::get(const std::string& endpoint) {
    CURL* curl = curl_easy_init();
    if (!curl)
        throw std::runtime_error("Failed to init curl");

    Response res{};
    std::string url = BASE_URL + endpoint;

    // sandbox mode — same URL as production, toggled by this header
    curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, "x-simulated-trading: 1");

    curl_easy_setopt(curl, CURLOPT_URL,            url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER,     headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,  write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA,      &res.body);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT,        10L);
    curl_easy_setopt(curl, CURLOPT_USERAGENT,      "market-connection/1.0");

    auto t0 = std::chrono::high_resolution_clock::now();

    CURLcode code = curl_easy_perform(curl);

    auto t1 = std::chrono::high_resolution_clock::now();
    res.elapsed_ms = std::chrono::duration<double, std::milli>(t1 - t0).count();

    curl_slist_free_all(headers);

    if (code != CURLE_OK) {
        curl_easy_cleanup(curl);
        throw std::runtime_error(curl_easy_strerror(code));
    }

    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &res.http_code);
    curl_easy_cleanup(curl);
    return res;
}
