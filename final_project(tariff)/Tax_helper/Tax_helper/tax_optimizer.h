#pragma once
#ifndef TAX_OPTIMIZER_H
#define TAX_OPTIMIZER_H

#define MAX_ITEMS 100
#define MAX_BOXES 20
#define MAX_NAME_LEN 128

typedef struct {
    char name[MAX_NAME_LEN];
    double price;      // 개당 가격 (원화 기준)
    int quantity;      // 수량
    char hs_code[10];  // 관세청 코드 (현재 사용되지 않지만, 구조체에 유지)
    double duty_rate;  // 관세율 (0.08 등)
    int is_processed;  // 이 품목이 이미 박스에 할당되었는지 여부를 나타내는 플래그 (0: 아니오, 1: 예)
} Item;

typedef struct {
    Item* items[MAX_ITEMS];
    int count;
    double total_price;
    double total_tax;
} Box;

// 외부에서 접근 가능한 변수
extern Item item_list[MAX_ITEMS];
extern int item_count;
extern Box boxes[MAX_BOXES];
extern int box_count;

// 최적화 알고리즘 함수
void loadTaxItemsOpt(const char* csvPath);
void selectcur();
void build_duty_free_boxes();
void build_duty_boxes();
void print_boxes();
void reset_optimizer();
void handleTaxCalculation_Optmize();

#endif