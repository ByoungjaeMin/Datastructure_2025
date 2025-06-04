#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "tax_api.h"

#define API_KEY ""  // ← 여기에 본인의 API Key 입력
#define API_URL "https://unipass.customs.go.kr:38010/ext/rest/hscdTariffInfoQry/retrieveHscdTariffInfo"

struct MemoryStruct {
    char* memory;
    size_t size;
};

// 응답 버퍼에 데이터 누적
static size_t WriteMemoryCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t realsize = size * nmemb;
    struct MemoryStruct* mem = (struct MemoryStruct*)userp;

    char* ptr = realloc(mem->memory, mem->size + realsize + 1);
    if (!ptr) return 0;

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = '\0';
    return realsize;
}

// XML 응답에서 <dutyRate> 값 추출
char* parse_tariff_rate(const char* xml) {
    static char rate[16] = { 0 };
    const char* start = strstr(xml, "<dutyRate>");
    if (start) {
        start += strlen("<dutyRate>");
        const char* end = strstr(start, "</dutyRate>");
        if (end && end - start < sizeof(rate)) {
            strncpy(rate, start, end - start);
            rate[end - start] = '\0';
        }
    }
    return rate;
}

// 내부 API 호출 및 결과 반환
char* get_tariff_rate_from_api(const char* hs_code) {
    CURL* curl;
    CURLcode res;

    struct MemoryStruct chunk;
    chunk.memory = malloc(1);
    chunk.size = 0;

    char url[512];
    snprintf(url, sizeof(url), "%s?crkyCn=%s&hsSgn=%s", API_URL, API_KEY, hs_code);

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&chunk);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "[ERROR] curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }
        else {
            printf("\n[DEBUG] API 응답 원문 ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓\n");
            printf("%s\n", chunk.memory);
            printf("[DEBUG] ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑\n");
        }

        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();

    return parse_tariff_rate(chunk.memory);
}


// 외부에 제공되는 출력 함수
void fetch_tariff_from_api(const char* hs_code) {
    char* duty = get_tariff_rate_from_api(hs_code);
    if (strlen(duty) > 0) {
        printf("HS코드 %s 의 관세율은 %s%% 입니다.\n", hs_code, duty);
    }
    else {
        printf("관세율 정보를 불러오는 데 실패했습니다. HS코드를 다시 확인하세요.\n");
    }
}
