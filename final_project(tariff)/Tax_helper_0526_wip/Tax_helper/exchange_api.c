#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "exchange_api.h"

#define API_KEY "(API)"
#define API_URL "https://unipass.customs.go.kr:38010/ext/rest/trtyCmmnRtsQry/retrieveTrtyCmmnRts"

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    strcat((char*)userp, (char*)contents);
    return size * nmemb;
}

double get_usd_to_krw_rate() {
    CURL* curl = curl_easy_init();
    static char buffer[2048] = { 0 };

    if (curl) {
        char url[512];
        snprintf(url, sizeof(url), "%s?crkyCn=%s", API_URL, API_KEY);
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, buffer);
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }

    // 응답 예시: <cmmnCd>USD</cmmnCd>...<krwCnvtRts>1350.30</krwCnvtRts>
    char* pos = strstr(buffer, "<cmmnCd>USD</cmmnCd>");
    if (pos) {
        char* rate_start = strstr(pos, "<krwCnvtRts>");
        if (rate_start) {
            rate_start += strlen("<krwCnvtRts>");
            char* rate_end = strstr(rate_start, "</krwCnvtRts>");
            if (rate_end) {
                char rate_str[16] = { 0 };
                strncpy(rate_str, rate_start, rate_end - rate_start);
                return atof(rate_str);
            }
        }
    }

    return -1.0;
}