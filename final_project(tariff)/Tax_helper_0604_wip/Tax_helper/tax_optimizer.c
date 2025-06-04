#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "tax_optimizer.h"
#include "tax_calc.h"
#include "exchange_api.h"

#define MAX_LINE 512
#define MAX_ITEM 512
#define MAX_CATEGORY 64
#define MAX_CHILD_PER_CAT 256

static TaxItem taxListOpt[MAX_ITEM];
static int taxCountOpt = 0;

static char categoryNamesOpt[MAX_CATEGORY][MAX_NAME_LEN];
static int categoryCountOpt = 0;

static int catItemIndexOpt[MAX_CATEGORY][MAX_CHILD_PER_CAT];
static int catItemCountOpt[MAX_CATEGORY];

Item item_list[MAX_ITEMS];
int item_count = 0;
Box boxes[MAX_BOXES];
int box_count = 0;

static double parsePercentOpt(const char* percentStr) {
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

void loadTaxItemsOpt(const char* csvPath) {
    FILE* fp = fopen(csvPath, "r");
    if (fp == NULL) {
        perror("Failed to open tax_items.csv");
        return;
    }

    char line[MAX_LINE];
    taxCountOpt = 0;
    categoryCountOpt = 0;
    for (int i = 0; i < MAX_CATEGORY; ++i) {
        catItemCountOpt[i] = 0;
    }

    if (fgets(line, sizeof(line), fp) == NULL) {
        fclose(fp);
        return;
    }

    while (fgets(line, sizeof(line), fp) != NULL && taxCountOpt < MAX_ITEM) {
        line[strcspn(line, "\r\n")] = '\0';

        char* token = strtok(line, ",");
        if (token) strncpy(taxListOpt[taxCountOpt].taxType, token, MAX_NAME_LEN - 1);
        taxListOpt[taxCountOpt].taxType[MAX_NAME_LEN - 1] = '\0';

        token = strtok(NULL, ",");
        if (token) strncpy(taxListOpt[taxCountOpt].itemName, token, MAX_NAME_LEN - 1);
        taxListOpt[taxCountOpt].itemName[MAX_NAME_LEN - 1] = '\0';

        token = strtok(NULL, ",");
        taxListOpt[taxCountOpt].tariffRate = token ? parsePercentOpt(token) : 0.0;

        token = strtok(NULL, ",");
        taxListOpt[taxCountOpt].vatRate = token ? parsePercentOpt(token) : 0.0;

        token = strtok(NULL, ",");
        taxListOpt[taxCountOpt].specialRate = token ? parsePercentOpt(token) : 0.0;

        int categoryIdx = -1;
        for (int i = 0; i < categoryCountOpt; ++i) {
            if (strcmp(categoryNamesOpt[i], taxListOpt[taxCountOpt].taxType) == 0) {
                categoryIdx = i;
                break;
            }
        }

        if (categoryIdx == -1) {
            if (categoryCountOpt < MAX_CATEGORY) {
                strncpy(categoryNamesOpt[categoryCountOpt], taxListOpt[taxCountOpt].taxType, MAX_NAME_LEN - 1);
                categoryNamesOpt[categoryCountOpt][MAX_NAME_LEN - 1] = '\0';
                categoryIdx = categoryCountOpt++;
            }
            else {
                continue;
            }
        }
        if (catItemCountOpt[categoryIdx] < MAX_CHILD_PER_CAT) {
            catItemIndexOpt[categoryIdx][catItemCountOpt[categoryIdx]++] = taxCountOpt;
        }
        taxCountOpt++;
    }
    fclose(fp);
}

void printCategoryListOpt() {
    printf("\n=== 카테고리 목록 ===\n");
    for (int i = 0; i < categoryCountOpt; ++i) {
        printf("%d. %s\n", i + 1, categoryNamesOpt[i]);
    }
    printf("====================\n");
}

void printItemsInCategoryOpt(int catNo) {
    if (catNo < 1 || catNo > categoryCountOpt) {
        printf("유효하지 않은 카테고리 번호입니다.\n");
        return;
    }

    printf("\n=== %s 품목 목록 ===\n", categoryNamesOpt[catNo - 1]);
    for (int i = 0; i < catItemCountOpt[catNo - 1]; ++i) {
        int itemIdx = catItemIndexOpt[catNo - 1][i];
        printf("%d. %s\n", i + 1, taxListOpt[itemIdx].itemName);
    }
    printf("=====================\n");
}

static int findTaxIndexByNameOpt(const char* itemName) {
    for (int i = 0; i < taxCountOpt; ++i) {
        if (strcmp(taxListOpt[i].itemName, itemName) == 0) {
            return i;
        }
    }
    return -1;
}

int compareItems(const void* a, const void* b) {
    Item* itemA = (Item*)a;
    Item* itemB = (Item*)b;
    double total_price_A = itemA->price * itemA->quantity;
    double total_price_B = itemB->price * itemB->quantity;

    if (total_price_A < total_price_B) return -1;
    if (total_price_A > total_price_B) return 1;
    return 0;
}

void build_duty_free_boxes() {
    qsort(item_list, item_count, sizeof(Item), compareItems);

    double usd_to_krw_rate = get_usd_to_krw_rate();
    if (usd_to_krw_rate == 0) {
        fprintf(stderr, "오류: 환율 정보를 가져올 수 없습니다. 박스 구성에 실패했습니다.\n");
        return;
    }

    box_count = 0;

    for (int i = 0; i < item_count; ++i) {
        if (item_list[i].is_processed) continue;

        double item_total_krw = item_list[i].price * item_list[i].quantity;
        double item_total_usd = item_total_krw / usd_to_krw_rate;

        int placed_in_existing_box = 0;
        for (int b = 0; b < box_count; ++b) {
            if ((boxes[b].total_price + item_total_krw) / usd_to_krw_rate <= DUTY_FREE_LIMIT && boxes[b].count < MAX_ITEMS) {
                boxes[b].items[boxes[b].count++] = &item_list[i];
                boxes[b].total_price += item_total_krw;
                item_list[i].is_processed = 1;
                placed_in_existing_box = 1;
                break;
            }
        }

        if (!placed_in_existing_box) {
            if (box_count < MAX_BOXES) {
                boxes[box_count].total_price = 0.0;
                boxes[box_count].total_tax = 0.0;
                boxes[box_count].count = 0;

                if (boxes[box_count].count < MAX_ITEMS) {
                    boxes[box_count].items[boxes[box_count].count++] = &item_list[i];
                    boxes[box_count].total_price += item_total_krw;
                    item_list[i].is_processed = 1;
                    box_count++;
                }
            }
            else {
                fprintf(stderr, "경고: 최대 박스 개수를 초과했습니다. 더 이상 면세 박스를 생성할 수 없습니다.\n");
                break;
            }
        }
    }
}

void build_duty_boxes() {
    double usd_to_krw_rate = get_usd_to_krw_rate();
    if (usd_to_krw_rate == 0) {
        fprintf(stderr, "오류: 환율 정보를 가져올 수 없습니다. 박스 구성에 실패했습니다.\n");
        return;
    }

    for (int i = 0; i < item_count; ++i) {
        if (item_list[i].is_processed) continue;

        if (box_count >= MAX_BOXES) {
            fprintf(stderr, "경고: 최대 박스 개수를 초과했습니다. 더 이상 과세 박스를 생성할 수 없습니다.\n");
            break;
        }

        boxes[box_count].total_price = 0.0;
        boxes[box_count].total_tax = 0.0;
        boxes[box_count].count = 0;

        boxes[box_count].items[boxes[box_count].count++] = &item_list[i];
        boxes[box_count].total_price += item_list[i].price * item_list[i].quantity;
        item_list[i].is_processed = 1;

        box_count++;
    }
}

void print_boxes() {
    printf("\n===== 관부가세 최적화 결과 =====\n");
    double usd_to_krw_rate = get_usd_to_krw_rate();
    if (usd_to_krw_rate == 0) {
        fprintf(stderr, "오류: 환율 정보를 가져올 수 없습니다. 정확한 면세 정보를 표시할 수 없습니다.\n");
    }

    for (int i = 0; i < box_count; ++i) {
        printf("\n📦 박스 %d:\n", i + 1);
        printf("  포함된 품목:\n");
        for (int j = 0; j < boxes[i].count; ++j) {
            Item* it = boxes[i].items[j];
            printf("    - %s (수량: %d): 개당 %.0f원 (총 %.0f원)\n", it->name, it->quantity, it->price, it->price * it->quantity);
        }
        printf("  총 상품 가격: %.0f원\n", boxes[i].total_price);
        if (usd_to_krw_rate != 0) {
            printf("  총 상품 가격 (USD): $%.2f\n", boxes[i].total_price / usd_to_krw_rate);
        }

        double box_total_usd = boxes[i].total_price / usd_to_krw_rate;

        if (box_total_usd > DUTY_FREE_LIMIT) {
            double actual_box_tax = 0.0;
            for (int j = 0; j < boxes[i].count; ++j) {
                Item* it = boxes[i].items[j];
                int tax_idx = findTaxIndexByNameOpt(it->name);
                if (tax_idx != -1) {
                    actual_box_tax += calculate_tax_for_item(
                        it->price * it->quantity,
                        taxListOpt[tax_idx].tariffRate,
                        taxListOpt[tax_idx].vatRate,
                        taxListOpt[tax_idx].specialRate
                    );
                }
                else {
                    actual_box_tax += it->price * it->quantity * (it->duty_rate / 100.0);
                }
            }
            printf("  예상 관세액: %.0f원\n", actual_box_tax);
            printf("  (과세 대상 - 면세 한도 초과)\n");
        }
        else {
            printf("  예상 관세액: 0원\n");
            printf("  (면세 한도 내에 포함)\n");
        }
    }
    printf("==================================\n");
}

void reset_optimizer() {
    box_count = 0;
    item_count = 0;
    for (int i = 0; i < MAX_ITEMS; ++i) {
        item_list[i].is_processed = 0;
    }
    for (int i = 0; i < MAX_BOXES; ++i) {
        boxes[i].count = 0;
        boxes[i].total_price = 0.0;
        boxes[i].total_tax = 0.0;
    }
}

void handleTaxCalculation_Optmize(void) {
    int mode = 0;
    printf("1) 품목명 직접 입력  2) 카테고리 목록 보기\n");
    printf("선택(1 또는 2): ");
    if (scanf("%d", &mode) != 1) {
        printf("잘못된 입력입니다. 숫자를 입력해주세요.\n");
        while (getchar() != '\n');
        return;
    }
    while (getchar() != '\n');

    char product[MAX_NAME_LEN] = { 0 };

    if (mode == 2) {
        printCategoryListOpt();
        int catNo;
        printf("카테고리 번호를 입력하세요: ");
        if (scanf("%d", &catNo) != 1 || catNo < 1 || catNo > categoryCountOpt) {
            printf("잘못된 카테고리 번호입니다.\n");
            while (getchar() != '\n');
            return;
        }
        while (getchar() != '\n');

        printItemsInCategoryOpt(catNo);
        int itemNo;
        printf("품목 번호를 입력하세요: ");
        if (scanf("%d", &itemNo) != 1 ||
            itemNo < 1 || itemNo > catItemCountOpt[catNo - 1]) {
            printf("잘못된 품목 번호입니다.\n");
            while (getchar() != '\n');
            return;
        }
        while (getchar() != '\n');

        int taxIdx = catItemIndexOpt[catNo - 1][itemNo - 1];
        strncpy(product, taxListOpt[taxIdx].itemName, MAX_NAME_LEN - 1);
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
    double unitPrice = 0.0;

    printf("수량을 입력하세요: ");
    if (scanf("%d", &quantity) != 1) {
        printf("잘못된 수량입니다.\n");
        while (getchar() != '\n');
        return;
    }

    printf("단가를 입력하세요 (원화 기준, 예: 1500000): ");
    if (scanf("%lf", &unitPrice) != 1) {
        printf("잘못된 단가입니다.\n");
        while (getchar() != '\n');
        return;
    }
    while (getchar() != '\n');

    int idx = findTaxIndexByNameOpt(product);
    if (idx < 0) {
        printf("해당 품목을 찾을 수 없습니다.\n");
        return;
    }

    if (item_count < MAX_ITEMS) {
        strncpy(item_list[item_count].name, product, sizeof(item_list[item_count].name) - 1);
        item_list[item_count].name[sizeof(item_list[item_count].name) - 1] = '\0';
        item_list[item_count].price = unitPrice;
        item_list[item_count].quantity = quantity;
        item_list[item_count].duty_rate = taxListOpt[idx].tariffRate;
        item_list[item_count].is_processed = 0;
        item_count++;
        printf("품목 '%s'가 최적화 목록에 추가되었습니다.\n\n", product);
    }
    else {
        printf("더 이상 품목을 추가할 수 없습니다 (최대 %d개).\n", MAX_ITEMS);
    }
}