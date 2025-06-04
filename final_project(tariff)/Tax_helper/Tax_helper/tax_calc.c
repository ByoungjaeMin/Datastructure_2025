#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tax_calc.h"

#define MAX_LINE 512
#define MAX_ITEM 512
#define MAX_CATEGORY 64
#define MAX_CHILD_PER_CAT 256

static TaxItem taxList[MAX_ITEM];
static int taxCount = 0;

static char categoryNames[MAX_CATEGORY][MAX_NAME_LEN];
static int categoryCount = 0;

static int catItemIndex[MAX_CATEGORY][MAX_CHILD_PER_CAT];
static int catItemCount[MAX_CATEGORY];

static double parsePercent(const char* percentStr) {
    char buf[MAX_NAME_LEN];
    int i, j = 0;
    for (i = 0; percentStr[i] != '\0' && j < (int)sizeof(buf) - 1; ++i) {
        if (percentStr[i] == '%') continue;
        if (percentStr[i] == '/') break;
        if (percentStr[i] == ' ') continue;
        buf[j++] = percentStr[i];
    }
    buf[j] = '\0';
    return atof(buf);
}

void loadTaxItems(const char* csvPath) {
    FILE* fp = fopen(csvPath, "r");
    if (fp == NULL) {
        fprintf(stderr, "ERROR: CSV ������ �� �� �����ϴ�: %s\n", csvPath);
        taxCount = 0;
        return;
    }

    char line[MAX_LINE];
    taxCount = 0;
    categoryCount = 0;
    for (int i = 0; i < MAX_CATEGORY; i++) {
        catItemCount[i] = 0;
    }

    // ù ��(���) �ǳʶٱ�
    if (fgets(line, sizeof(line), fp) == NULL) {
        fclose(fp);
        return;
    }

    while (fgets(line, sizeof(line), fp) != NULL) {
        line[strcspn(line, "\r\n")] = '\0';
        if (line[0] == '\0') continue;

        char* p1 = strrchr(line, ',');
        if (!p1) continue;
        char specialStr[MAX_NAME_LEN];
        strcpy(specialStr, p1 + 1);
        *p1 = '\0';

        char* p2 = strrchr(line, ',');
        if (!p2) continue;
        char vatStr[MAX_NAME_LEN];
        strcpy(vatStr, p2 + 1);
        *p2 = '\0';

        char* p3 = strrchr(line, ',');
        if (!p3) continue;
        char tariffStr[MAX_NAME_LEN];
        strcpy(tariffStr, p3 + 1);
        *p3 = '\0';

        char* comma = strchr(line, ',');
        if (!comma) continue;
        *comma = '\0';
        char* taxType = line;
        char* itemName = comma + 1;
        while (*itemName == ' ') itemName++;

        double tariffRate = parsePercent(tariffStr);
        double vatRate = parsePercent(vatStr);
        double specialRate = parsePercent(specialStr);

        strncpy(taxList[taxCount].taxType, taxType, MAX_NAME_LEN - 1);
        strncpy(taxList[taxCount].itemName, itemName, MAX_NAME_LEN - 1);
        taxList[taxCount].tariffRate = tariffRate;
        taxList[taxCount].vatRate = vatRate;
        taxList[taxCount].specialRate = specialRate;

        int catIdx = -1;
        for (int i = 0; i < categoryCount; i++) {
            if (strcmp(categoryNames[i], taxType) == 0) {
                catIdx = i;
                break;
            }
        }
        if (catIdx < 0 && categoryCount < MAX_CATEGORY) {
            catIdx = categoryCount;
            strncpy(categoryNames[catIdx], taxType, MAX_NAME_LEN - 1);
            categoryCount++;
        }

        if (catIdx >= 0 && catItemCount[catIdx] < MAX_CHILD_PER_CAT) {
            catItemIndex[catIdx][catItemCount[catIdx]++] = taxCount;
        }

        taxCount++;
        if (taxCount >= MAX_ITEM - 1) break;
    }

    fclose(fp);
}

// ����������������������������������������������������������������������������������������������������������������������������������������������������������
// ī�װ� ��ϸ� ����
static void printCategoryList() {
    printf("\n====== ī�װ� ��� ======\n");
    for (int i = 0; i < categoryCount; i++) {
        printf("[%d] %s\n", i + 1, categoryNames[i]);
    }
    printf("==========================\n\n");
}

// ������ ī�װ��� ���� ǰ�� ����
static void printItemsInCategory(int catNo) {
    if (catNo < 1 || catNo > categoryCount) return;
    int idx = catNo - 1;
    printf("\n-- [%s] ī�װ��� ǰ��� --\n", categoryNames[idx]);
    for (int j = 0; j < catItemCount[idx]; j++) {
        int tIdx = catItemIndex[idx][j];
        printf("    (%d) %s\n", j + 1, taxList[tIdx].itemName);
    }
    printf("------------------------\n\n");
}

// ����������������������������������������������������������������������������������������������������������������������������������������������������������
// itemName���� taxList �ε����� ã�� �ִ� �Լ�
static int findTaxIndexByName(const char* itemName) {
    for (int i = 0; i < taxCount; i++) {
        if (strcmp(taxList[i].itemName, itemName) == 0) {
            return i;
        }
    }
    return -1;
}

// ����������������������������������������������������������������������������������������������������������������������������������������������������������
// �޴� 3��: ���� ���
void handleTaxCalculation(void) {
    if (taxCount == 0) {
        printf("���� �����Ͱ� �ε���� �ʾҽ��ϴ�.\n");
        return;
    }

    int mode = 0;
    printf("1) ǰ��� ���� �Է�  2) ī�װ� ��� ����\n");
    printf("����(1 or 2): ");
    if (scanf("%d", &mode) != 1) {
        printf("�߸��� �Է��Դϴ�.\n");
        while (getchar() != '\n');
        return;
    }
    while (getchar() != '\n');

    char product[MAX_NAME_LEN] = { 0 };

    if (mode == 2) {
        // 1) ī�װ� ��ϸ� ���
        printCategoryList();
        int catNo;
        printf("ī�װ� ��ȣ�� �Է��ϼ���: ");
        if (scanf("%d", &catNo) != 1 || catNo < 1 || catNo > categoryCount) {
            printf("�߸��� ī�װ� ��ȣ�Դϴ�.\n");
            while (getchar() != '\n');
            return;
        }
        while (getchar() != '\n');

        // 2) ���õ� ī�װ��� ǰ�� ���
        printItemsInCategory(catNo);
        int itemNo;
        printf("ǰ�� ��ȣ�� �Է��ϼ���: ");
        if (scanf("%d", &itemNo) != 1 ||
            itemNo < 1 || itemNo > catItemCount[catNo - 1]) {
            printf("�߸��� ǰ�� ��ȣ�Դϴ�.\n");
            while (getchar() != '\n');
            return;
        }
        while (getchar() != '\n');

        int taxIdx = catItemIndex[catNo - 1][itemNo - 1];
        strncpy(product, taxList[taxIdx].itemName, MAX_NAME_LEN - 1);
        product[MAX_NAME_LEN - 1] = '\0';
        printf("���õ� ǰ��: %s\n", product);
    }
    else if (mode == 1) {
        printf("��ǰ���� ��Ȯ�� �Է��ϼ��� (��: ����(200���� ����)): ");
        if (fgets(product, sizeof(product), stdin) == NULL) {
            printf("�Է� ����\n");
            return;
        }
        product[strcspn(product, "\r\n")] = '\0';
    }
    else {
        printf("�߸��� �����Դϴ�.\n");
        return;
    }

    int quantity = 0;
    double unitPriceThousand = 0.0;

    printf("������ �Է��ϼ���: ");
    if (scanf("%d", &quantity) != 1) {
        printf("�߸��� �����Դϴ�.\n");
        while (getchar() != '\n');
        return;
    }

    printf("�ܰ��� �Է��ϼ��� (õ�� ����, ��: 1500 �� 1,500,000��): ");
    if (scanf("%lf", &unitPriceThousand) != 1) {
        printf("�߸��� �ܰ��Դϴ�.\n");
        while (getchar() != '\n');
        return;
    }
    while (getchar() != '\n');

    int idx = findTaxIndexByName(product);
    if (idx < 0) {
        printf("�ش� ǰ���� ã�� �� �����ϴ�. ��Ȯ�� �̸��� Ȯ���ϼ���.\n");
        return;
    }

    double unitPrice = unitPriceThousand * 1000.0;
    double totalPrice = unitPrice * quantity;

    double tariffAmt = totalPrice * (taxList[idx].tariffRate / 100.0);
    double vatAmt = totalPrice * (taxList[idx].vatRate / 100.0);
    double specialAmt = totalPrice * (taxList[idx].specialRate / 100.0);
    double totalTax = tariffAmt + vatAmt + specialAmt;
    double finalPrice = totalPrice + totalTax;

    printf("\n=== [���� ��� ���] ===\n");
    printf("ī�װ�: %s\n", taxList[idx].taxType);
    printf("ǰ���: %s\n", product);
    printf("����: %d\n", quantity);
    printf("�ܰ�(��): %.0f\n", unitPrice);
    printf("�� ��ǰ����(��): %.0f\n", totalPrice);
    printf("������: %.1f%% �� ������: %.0f��\n", taxList[idx].tariffRate, tariffAmt);
    printf("�ΰ�����: %.1f%% �� �ΰ�����: %.0f��\n", taxList[idx].vatRate, vatAmt);
    printf("Ư�Ҽ���: %.1f%% �� Ư�Ҽ���: %.0f��\n", taxList[idx].specialRate, specialAmt);
    printf("�� ���μ���(��): %.0f\n", totalTax);
    printf("���� ���αݾ�(��): %.0f\n\n", finalPrice);
}
