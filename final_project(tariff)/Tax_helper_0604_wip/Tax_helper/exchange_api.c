#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define API_URL  "https://unipass.customs.go.kr:38010/ext/rest/trifFxrtInfoQry/retrieveTrifFxrtInfo"
#define API_KEY  "s220a245c035i225y080w060e0"

double get_usd_to_krw_rate() {
    FILE* fp_curl;
    char command[512];
    char* response_buffer = NULL;
    size_t current_buffer_size = 0;
    //api 시간동기화(일단위)
    time_t now = time(NULL);
    struct tm* nowStruct;
    nowStruct = localtime(&now);

    snprintf(command, sizeof(command), "curl -s \"%s?crkyCn=%s&qryYymmDd=%d%02d%02d&imexTp=2\"", API_URL, API_KEY, nowStruct->tm_year + 1900 , nowStruct->tm_mon + 1, nowStruct->tm_mday);


    fp_curl = _popen(command, "r");
    if (fp_curl == NULL) {
        perror("Failed to run curl command");
        return 1;
    }

    current_buffer_size = 12000;
    response_buffer = (char*)malloc(current_buffer_size);
    if (response_buffer == NULL) {
        perror("Memory allocation failed");
        _pclose(fp_curl);
        return 1;
    }
    response_buffer[0] = '\0';

    fgets(response_buffer, current_buffer_size, fp_curl);

    int pclose_status = _pclose(fp_curl);
    if (pclose_status == -1) {
        perror("Error closing pipe");
    }

    char* pos = strstr(response_buffer, "<cntySgn>US</cntySgn>");
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