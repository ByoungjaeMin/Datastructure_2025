#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tax.h"

double calculate_tax(double price, const char* rate_str) {
	//세율은 예시로 20%로 설정, 실제로는 rate_str에서 가져와야 함
    double rate = 0.2;//atof(rate_str);
    double total = price * rate;
    printf("해당 물품에 청구되는 세금은 %f 원 입니다.\n", total);
}

// 트리 순회하며 카테고리/세율 출력
void show_tariff_by_category(TreeNode* root) {
    printf("카테고리별 관세/부가세율 목록:\n");
    print_tree(root, 0);
}

// 국가별 면세 범위 보기
void show_country_duty_free_info() {
    printf("\n국가별 면세 범위:\n");
    printf("- 미국: 200달러\n");
    printf("- 일본: 21,000엔 (약 150달러)\n");
    printf("- 유럽연합: 130유로 (약 150달러)\n");
    printf("- 한국(기준): 150달러\n");
    printf("(※ 배송료 포함 금액 기준이며, 전자제품/주류/식품은 별도 과세될 수 있음)\n");
}