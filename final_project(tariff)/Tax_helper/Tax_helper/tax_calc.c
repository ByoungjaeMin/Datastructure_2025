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

int infoOnlyMode = 0;

static double parsePercent(const char* percentStr) {
    char buf[MAX_NAME_LEN];
    int i, j = 0;
    for (i = 0; percentStr[i] != '\0' && j < MAX_NAME_LEN - 1; ++i) {
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
        fprintf(stderr, "ERROR: CSV 파일을 열 수 없습니다: %s\n", csvPath);
        taxCount = 0;
        return;
    }

    char line[MAX_LINE];
    taxCount = 0;
    categoryCount = 0;
    for (int i = 0; i < MAX_CATEGORY; i++) {
        catItemCount[i] = 0;
    }

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
        taxList[taxCount].taxType[MAX_NAME_LEN - 1] = '\0';
        strncpy(taxList[taxCount].itemName, itemName, MAX_NAME_LEN - 1);
        taxList[taxCount].itemName[MAX_NAME_LEN - 1] = '\0';
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
            categoryNames[catIdx][MAX_NAME_LEN - 1] = '\0';
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


void printCategoryList() {
    printf("\n====== 카테고리 목록 ======\n");
    for (int i = 0; i < categoryCount; i++) {
        printf("[%d] %s\n", i + 1, categoryNames[i]);
    }
    printf("==========================\n\n");
}


void printItemsInCategory(int catNo) {
    if (catNo < 1 || catNo > categoryCount) return;
    int idx = catNo - 1;
    printf("\n-- [%s] 카테고리의 품목들 --\n", categoryNames[idx]);
    for (int j = 0; j < catItemCount[idx]; j++) {
        int tIdx = catItemIndex[idx][j];
        printf("    (%d) %s\n", j + 1, taxList[tIdx].itemName);
    }
    printf("------------------------\n\n");
}

int findTaxIndexByName(const char* itemName) {
    for (int i = 0; i < taxCount; i++) {
        if (strcmp(taxList[i].itemName, itemName) == 0) {
            return i;
        }
    }
    return -1;
}

double calculate_tax_for_item(double price, double tariff_rate, double vat_rate, double special_rate) {
    double tariff_amount = price * (tariff_rate / 100.0);
    // 부가세는 물품 가격 + 관세에 부과됩니다.
    double vat_amount = (price + tariff_amount) * (vat_rate / 100.0);
    double special_amount = price * (special_rate / 100.0); // 특소세는 품목에 따라 다를 수 있습니다.

    return tariff_amount + vat_amount + special_amount;
}

void handleTaxCalculation(void) {
    if (taxCount == 0) {
        printf("세금 데이터가 로드되지 않았습니다.\n");
        return;
    }

    int mode = 0;
    printf("1) 품목명 직접 입력  2) 카테고리 목록 보기\n");
    printf("선택(1 or 2): ");
    if (scanf("%d", &mode) != 1) {
        printf("잘못된 입력입니다.\n");
        while (getchar() != '\n');
        return;
    }
    while (getchar() != '\n');

    char product[MAX_NAME_LEN] = { 0 };

    if (mode == 2) {
        printCategoryList();
        int catNo;
        printf("카테고리 번호를 입력하세요: ");
        if (scanf("%d", &catNo) != 1 || catNo < 1 || catNo > categoryCount) {
            printf("잘못된 카테고리 번호입니다.\n");
            while (getchar() != '\n');
            return;
        }
        while (getchar() != '\n');

        printItemsInCategory(catNo);
        int itemNo;
        printf("품목 번호를 입력하세요: ");
        if (scanf("%d", &itemNo) != 1 ||
            itemNo < 1 || itemNo > catItemCount[catNo - 1]) {
            printf("잘못된 품목 번호입니다.\n");
            while (getchar() != '\n');
            return;
        }
        while (getchar() != '\n');

        int taxIdx = catItemIndex[catNo - 1][itemNo - 1];
        strncpy(product, taxList[taxIdx].itemName, MAX_NAME_LEN - 1);
        product[MAX_NAME_LEN - 1] = '\0';
        printf("선택된 품목: %s\n", product);
    }
    else if (mode == 1) {
        printf("상품명을 정확히 입력하세요 (예: 가방(200만원 이하)): ");
        if (fgets(product, sizeof(product), stdin) == NULL) {
            printf("입력 오류\n");
            return;
        }
        product[strcspn(product, "\r\n")] = '\0';
    }
    else {
        printf("잘못된 선택입니다.\n");
        return;
    }

    int quantity = 0;
    double unitPriceThousand = 0.0;

    if (!infoOnlyMode) {
        printf("수량을 입력하세요: ");
        if (scanf("%d", &quantity) != 1) {
            printf("잘못된 수량입니다.\n");
            while (getchar() != '\n');
            return;
        }

        printf("단가를 입력하세요 (예: 1,500,000): ");
        if (scanf("%lf", &unitPriceThousand) != 1) {
            printf("잘못된 단가입니다.\n");
            while (getchar() != '\n');
            return;
        }
        while (getchar() != '\n');
    }


    int idx = findTaxIndexByName(product);
    if (idx < 0) {
        printf("해당 품목을 찾을 수 없습니다. 정확한 이름을 확인하세요.\n");
        return;
    }

    if (infoOnlyMode) {
        printf("선택된 품목: %s\n", product);
        TaxItem* ti = &taxList[idx];
        printf("\n=== [세율 정보] ===\n");
        printf("카테고리: %s\n", ti->taxType);
        printf("품목명   : %s\n", ti->itemName);
        printf("관세율   : %.1f%%\n", ti->tariffRate);
        printf("부가세율 : %.1f%%\n", ti->vatRate);
        printf("특소세율 : %.1f%%\n\n", ti->specialRate);
        return;    // 여기서 끝냄! 뒤의 수량/단가 입력 X
    }

    double unitPrice = unitPriceThousand * 1000.0;
    double totalPrice = unitPrice * quantity;

    double tariffAmt = totalPrice * (taxList[idx].tariffRate / 100.0);
    double vatAmt = totalPrice * (taxList[idx].vatRate / 100.0);
    double specialAmt = totalPrice * (taxList[idx].specialRate / 100.0);
    double totalTax = tariffAmt + vatAmt + specialAmt;
    double finalPrice = totalPrice + totalTax;

    printf("\n=== [세금 계산 결과] ===\n");
    printf("카테고리: %s\n", taxList[idx].taxType);
    printf("품목명: %s\n", product);
    printf("수량: %d\n", quantity);
    printf("단가(원): %.0f\n", unitPrice);
    printf("총 상품가격(원): %.0f\n", totalPrice);
    printf("관세율: %.1f%% → 관세액: %.0f원\n", taxList[idx].tariffRate, tariffAmt);
    printf("부가세율: %.1f%% → 부가세액: %.0f원\n", taxList[idx].vatRate, vatAmt);
    printf("특소세율: %.1f%% → 특소세액: %.0f원\n", taxList[idx].specialRate, specialAmt);
    printf("총 납부세액(원): %.0f\n", totalTax);
    printf("최종 납부금액(원): %.0f\n\n", finalPrice);
}