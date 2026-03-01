#include <curl/curl.h>

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <numeric>
#include <cmath>
#include <algorithm>

// ── types ──────────────────────────────────────────────────────────────────

struct EndpointConfig {
    std::string              exchange;
    std::string              label;
    std::string              url;
    std::vector<std::string> headers;
};

struct Sample {
    double elapsed_ms;
    long   http_code;
    bool   success;
};

struct Stats {
    double avg    = 0;
    double stddev = 0;
    double min    = 0;
    double max    = 0;
    int    ok     = 0;
    int    total  = 0;
};

// ── http ───────────────────────────────────────────────────────────────────

static size_t write_callback(char* ptr, size_t size, size_t nmemb, std::string* out) {
    out->append(ptr, size * nmemb);
    return size * nmemb;
}

static Sample fetch(const EndpointConfig& cfg) {
    CURL* curl = curl_easy_init();
    if (!curl) return {0, 0, false};

    std::string   body;
    curl_slist*   hlist = nullptr;
    for (const auto& h : cfg.headers)
        hlist = curl_slist_append(hlist, h.c_str());

    curl_easy_setopt(curl, CURLOPT_URL,           cfg.url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA,     &body);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT,       10L);
    curl_easy_setopt(curl, CURLOPT_USERAGENT,     "market-connection/1.0");
    if (hlist)
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, hlist);

    auto     t0   = std::chrono::high_resolution_clock::now();
    CURLcode code = curl_easy_perform(curl);
    auto     t1   = std::chrono::high_resolution_clock::now();

    if (hlist) curl_slist_free_all(hlist);

    Sample s{};
    s.elapsed_ms = std::chrono::duration<double, std::milli>(t1 - t0).count();
    s.success    = (code == CURLE_OK);
    if (s.success)
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &s.http_code);

    curl_easy_cleanup(curl);
    return s;
}

// ── stats ──────────────────────────────────────────────────────────────────

static Stats compute(const std::vector<Sample>& samples) {
    Stats st;
    st.total = static_cast<int>(samples.size());

    std::vector<double> vals;
    for (const auto& s : samples)
        if (s.success && s.http_code == 200) {
            vals.push_back(s.elapsed_ms);
            st.ok++;
        }

    if (vals.empty()) return st;

    st.avg = std::accumulate(vals.begin(), vals.end(), 0.0) / vals.size();
    st.min = *std::min_element(vals.begin(), vals.end());
    st.max = *std::max_element(vals.begin(), vals.end());

    double sq = 0;
    for (double v : vals) sq += (v - st.avg) * (v - st.avg);
    st.stddev = std::sqrt(sq / vals.size());

    return st;
}

// ── table ──────────────────────────────────────────────────────────────────

static std::string pad(const std::string& s, int w) {
    if ((int)s.size() >= w) return s.substr(0, w);
    return s + std::string(w - s.size(), ' ');
}

static void print_row(const std::string& exchange, const std::string& label, const Stats& st) {
    std::string rounds = std::to_string(st.ok) + "/" + std::to_string(st.total);

    std::cout << pad(exchange, 10)
              << pad(label, 14)
              << std::right
              << std::setw(8)  << rounds
              << std::fixed << std::setprecision(1)
              << std::setw(10) << st.avg
              << std::setw(10) << st.stddev
              << std::setw(10) << st.min
              << std::setw(10) << st.max
              << "\n";
}

// ── main ───────────────────────────────────────────────────────────────────

int main() {
    const int ROUNDS = 5;

    const std::vector<EndpointConfig> endpoints = {
        {"Binance",  "server_time",
            "https://testnet.binance.vision/api/v3/time",                                        {}},
        {"Binance",  "BTC/USDT",
            "https://testnet.binance.vision/api/v3/ticker/price?symbol=BTCUSDT",                 {}},

        {"Coinbase", "server_time",
            "https://api-public.sandbox.exchange.coinbase.com/time",                             {}},
        {"Coinbase", "BTC-USD",
            "https://api-public.sandbox.exchange.coinbase.com/products/BTC-USD/ticker",          {}},

        {"Kraken",   "server_time",
            "https://api.kraken.com/0/public/Time",                                              {}},
        {"Kraken",   "XBT/USD",
            "https://api.kraken.com/0/public/Ticker?pair=XBTUSD",                               {}},

        {"Upbit",    "market_list",
            "https://api.upbit.com/v1/market/all?isDetails=false",                               {}},
        {"Upbit",    "KRW-BTC",
            "https://api.upbit.com/v1/ticker?markets=KRW-BTC",                                  {}},

        {"OKX",      "server_time",
            "https://www.okx.com/api/v5/public/time",                                           {"x-simulated-trading: 1"}},
        {"OKX",      "BTC-USDT",
            "https://www.okx.com/api/v5/market/ticker?instId=BTC-USDT",                         {"x-simulated-trading: 1"}},

        {"Bybit",    "server_time",
            "https://api-testnet.bybit.com/v5/market/time",                                      {}},
        {"Bybit",    "BTC/USDT",
            "https://api-testnet.bybit.com/v5/market/tickers?category=spot&symbol=BTCUSDT",      {}},
    };

    curl_global_init(CURL_GLOBAL_DEFAULT);

    // ── collect ───────────────────────────────────────────────────────────
    std::cout << "Running " << ROUNDS << " rounds across 6 exchanges...\n\n";

    std::vector<std::pair<EndpointConfig, std::vector<Sample>>> results;
    for (const auto& ep : endpoints) {
        std::cout << "  " << ep.exchange << " / " << ep.label << " ..." << std::flush;
        std::vector<Sample> samples;
        for (int i = 0; i < ROUNDS; ++i)
            samples.push_back(fetch(ep));
        std::cout << " done\n";
        results.push_back({ep, std::move(samples)});
    }

    // ── table ─────────────────────────────────────────────────────────────
    const std::string sep(62, '-');
    std::cout << "\n" << sep << "\n";
    std::cout << pad("Exchange", 10)
              << pad("Endpoint", 14)
              << std::right
              << std::setw(8)  << "Rounds"
              << std::setw(10) << "Avg ms"
              << std::setw(10) << "Std Dev"
              << std::setw(10) << "Min"
              << std::setw(10) << "Max"
              << "\n"
              << sep << "\n";

    std::string last;
    for (const auto& [ep, samples] : results) {
        if (!last.empty() && ep.exchange != last)
            std::cout << "\n";
        last = ep.exchange;
        print_row(ep.exchange, ep.label, compute(samples));
    }

    std::cout << sep << "\n";

    curl_global_cleanup();
    return 0;
}
