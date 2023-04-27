#include <chrono>
#include <curl/curl.h>
#include <iostream>
#include <nlohmann/json.hpp>
#include <thread>

using json = nlohmann::json;

size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string *) userp)->append((char *) contents, size * nmemb);
    return size * nmemb;
}

double getPrice(int retries = 3) {
    for (int attempt = 1; attempt <= retries; ++attempt) {
        try {
            std::string readBuffer;
            CURL *curl;
            CURLcode res;

            curl_global_init(CURL_GLOBAL_DEFAULT);
            curl = curl_easy_init();

            if (curl) {
                curl_easy_setopt(curl, CURLOPT_URL,
                                 "https://api.coingecko.com/api/v3/simple/price?ids=ethereum&vs_currencies=usd");
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
                res = curl_easy_perform(curl);

                if (res != CURLE_OK) {
                    std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res)
                              << std::endl;
                }

                curl_easy_cleanup(curl);
            }

            curl_global_cleanup();

            json j = json::parse(readBuffer);
            double obtainedPriced = j["ethereum"]["usd"].get<double>();
            std::cout << obtainedPriced << std::endl;
            return obtainedPriced;
        } catch (const json::exception &e) {
            std::cerr << "Failed to fetch price. Attempt " << attempt << " of "
                      << retries << ". Error: " << e.what() << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(5));
        }
    }

    std::cerr << "Failed to fetch price after " << retries
              << " attempts. Exiting." << std::endl;
    std::exit(EXIT_FAILURE);
}

int main() {
    double previousPrice = getPrice();
    auto startTime = std::chrono::steady_clock::now();

    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(10));
        double currentPrice = getPrice();
        auto currentTime = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsedTime = currentTime - startTime;

        if (elapsedTime.count() >= 10.0) {
            double priceChange =
                    ((currentPrice - previousPrice) / previousPrice) * 100;

            if (std::abs(priceChange) >= 0.01) {
                std::cout << "ETHUSDT futures price has changed by " << priceChange
                          << "% in the last 10 seconds." << std::endl;
            }

            previousPrice = currentPrice;
            startTime = currentTime;
        }
    }

    return 0;
}
