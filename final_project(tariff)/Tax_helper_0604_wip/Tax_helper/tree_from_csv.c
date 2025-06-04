#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"
#include "tree_from_csv.h"

TreeNode* build_tree_from_csv(const char* filename) {
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        perror("CSV 파일 열기 실패");
        return NULL;
    }

    char line[512];
    fgets(line, sizeof(line), fp);  // 첫 줄(헤더) 건너뛰기

    TreeNode* root = NULL;

    while (fgets(line, sizeof(line), fp)) {
        // CSV는 "세금종류,품목,관세율,부가세율" 형식
        char category[64] = "", item[128] = "", tariff[32] = "-", vat[32] = "-";
        int parsed = sscanf(line, "%63[^,],%127[^,],%31[^,],%31[^\n[\r]]",
            category, item, tariff, vat);
        if (parsed < 2) continue;  // 최소 세금종류+품목이 있어야 의미 있음

        // "관세율: %s, 부가세율: %s" 형태의 문자열을 만듦
        char rate_data[128];
        snprintf(rate_data, sizeof(rate_data), "관세율:%s, 부가세율:%s", tariff, vat);

        // 트리에 삽입할 경로(key)는 "세금종류/품목" 형태
        char full_path[192];
        snprintf(full_path, sizeof(full_path), "%s/%s", category, item);

        root = insert_node(root, full_path, rate_data);
    }

    fclose(fp);
    return root;
}

