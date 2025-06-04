#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "tax_api.h"

#define API_KEY ""  // �� ���⿡ ������ API Key �Է�
#define API_URL "https://unipass.customs.go.kr:38010/ext/rest/hscdTariffInfoQry/retrieveHscdTariffInfo"

struct MemoryStruct {
    char* memory;
    size_t size;
};

// ���� ���ۿ� ������ ����
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

// XML ���信�� <dutyRate> �� ����
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

// ���� API ȣ�� �� ��� ��ȯ
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
            printf("\n[DEBUG] API ���� ���� ���������������������������������������\n");
            printf("%s\n", chunk.memory);
            printf("[DEBUG] ������������������������������������������������\n");
        }

        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();

    return parse_tariff_rate(chunk.memory);
}


// �ܺο� �����Ǵ� ��� �Լ�
void fetch_tariff_from_api(const char* hs_code) {
    char* duty = get_tariff_rate_from_api(hs_code);
    if (strlen(duty) > 0) {
        printf("HS�ڵ� %s �� �������� %s%% �Դϴ�.\n", hs_code, duty);
    }
    else {
        printf("������ ������ �ҷ����� �� �����߽��ϴ�. HS�ڵ带 �ٽ� Ȯ���ϼ���.\n");
    }
}
