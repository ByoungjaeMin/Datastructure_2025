#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tax.h"

double calculate_tax(double price, const char* rate_str) {
    double rate = atof(rate_str);
    return price * rate;
}

// 트리 순회하며 카테고리/세율 출력
void show_tariff_by_category(TreeNode* root) {
    printf("카테고리별 관세/부가세율 목록:\n");
    print_tree(root, 0);
}

// 국가별 면세 범위 보기
void show_country_duty_free_info() {
    printf("국가별 면세 범위:\n");
    printf("- 미국: 200달러\n");
    printf("- 일본: 10,000엔 (약 90달러)\n");
    printf("- 독일: 150유로 (약 160달러)\n");
    printf("- 한국(기준): 150달러\n");
    printf("(※ 배송료 포함 금액 기준이며, 전자제품/주류/식품은 별도 과세될 수 있음)\n");
}
