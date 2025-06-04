#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"
#include "tree_from_csv.h"

TreeNode* build_tree_from_csv(const char* filename) {
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        perror("CSV ���� ���� ����");
        return NULL;
    }

    char line[512];
    fgets(line, sizeof(line), fp);  // ù ��(���) �ǳʶٱ�

    TreeNode* root = NULL;

    while (fgets(line, sizeof(line), fp)) {
        // CSV�� "��������,ǰ��,������,�ΰ�����" ����
        char category[64] = "", item[128] = "", tariff[32] = "-", vat[32] = "-";
        int parsed = sscanf(line, "%63[^,],%127[^,],%31[^,],%31[^\n[\r]]",
            category, item, tariff, vat);
        if (parsed < 2) continue;  // �ּ� ��������+ǰ���� �־�� �ǹ� ����

        // "������: %s, �ΰ�����: %s" ������ ���ڿ��� ����
        char rate_data[128];
        snprintf(rate_data, sizeof(rate_data), "������:%s, �ΰ�����:%s", tariff, vat);

        // Ʈ���� ������ ���(key)�� "��������/ǰ��" ����
        char full_path[192];
        snprintf(full_path, sizeof(full_path), "%s/%s", category, item);

        root = insert_node(root, full_path, rate_data);
    }

    fclose(fp);
    return root;
}

