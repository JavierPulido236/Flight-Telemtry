#include <stdio.h>
#include <curl/curl.h>
#include "reporter.h"

void send_telemetry_report(TelemetryFrame frame) {
    CURL *curl = curl_easy_init();

    if (curl) {
        char json[512];

        snprintf(json, sizeof(json),
            "{"
            "\"altitude\": %.1f,"
            "\"airspeed\": %.1f,"
            "\"heading\": %.1f,"
            "\"fuel_level\": %.1f,"
            "\"engine_temp\": %.1f,"
            "\"failure_mode\": %d"
            "}",
            frame.altitude,
            frame.airspeed,
            frame.heading,
            frame.fuel_level,
            frame.engine_temp,
            frame.failure_mode
        );

        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl, CURLOPT_URL, "https://httpbin.org/post");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json);

        CURLcode res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            printf("request failed\n");
        }

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }
}