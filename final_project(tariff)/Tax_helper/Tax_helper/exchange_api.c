#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "exchange_api.h" // 변경된 헤더 파일 포함

#define API_URL  "https://unipass.customs.go.kr:38010/ext/rest/trifFxrtInfoQry/retrieveTrifFxrtInfo"
#define API_KEY  "API" //UNIPASS 관세환율정보조회 API 키 발급 필요

// get_rate 함수를 범용적으로 사용하여 특정 국가의 환율을 가져오도록 수정
double get_rate(const char* country_sign) {
    FILE* fp_curl;
    char command[512];
    char* response_buffer = NULL;
    size_t current_buffer_size = 0;

    // API 시간 동기화 (일 단위)
    time_t now = time(NULL);
    struct tm* nowStruct;
    nowStruct = localtime(&now);

    snprintf(command, sizeof(command), "curl -s \"%s?crkyCn=%s&qryYymmDd=%d%02d%02d&imexTp=2\"", API_URL, API_KEY, nowStruct->tm_year + 1900, nowStruct->tm_mon + 1, nowStruct->tm_mday);

    fp_curl = _popen(command, "r");
    if (fp_curl == NULL) {
        perror("Failed to run curl command");
        return 0.0; // 실패 시 0.0 반환
    }

    current_buffer_size = 12000;
    response_buffer = (char*)malloc(current_buffer_size);
    if (response_buffer == NULL) {
        perror("Memory allocation failed");
        _pclose(fp_curl);
        return 0.0; // 실패 시 0.0 반환
    }
    response_buffer[0] = '\0';
    fgets(response_buffer, current_buffer_size, fp_curl);

    int pclose_status = _pclose(fp_curl);
    if (pclose_status == -1) {
        perror("Error closing pipe");
    }

    char search_tag[32];
    snprintf(search_tag, sizeof(search_tag), "<cntySgn>%s</cntySgn>", country_sign);

    char* pos = strstr(response_buffer, search_tag);
    char rate_str[16] = { 0 };
    if (pos) {
        char* rate_start = strstr(pos, "<fxrt>");
        if (rate_start) {
            rate_start += strlen("<fxrt>");
            char* rate_end = strstr(rate_start, "</fxrt>");
            if (rate_end) {
                strncpy(rate_str, rate_start, rate_end - rate_start);
            }
        }
    }
    free(response_buffer);

    return atof(rate_str);
}

double get_usd_to_krw_rate() {
    return get_rate("US");
}

double get_eur_to_krw_rate() {
    return get_rate("EU");
}

double get_jpy_to_krw_rate() {
    return get_rate("JP");
}

double get_cny_to_krw_rate() {
    return get_rate("CN");
}
