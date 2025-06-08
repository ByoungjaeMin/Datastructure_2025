#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tax.h"
#include "exchange_api.h"

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

// 국가별 환율, 면세 범위 보기
void show_country_rate_and_duty_free_info() {
    printf("\n주요 4개국 환율 정보\n");
	printf("- 미국: 1달러 = %.2f원\n", get_usd_to_krw_rate());
	printf("- 유럽연합: 1유로 = %.2f원\n", get_eur_to_krw_rate());
	printf("- 일본: 1엔 = %.2f원\n", get_jpy_to_krw_rate());
	printf("- 중국: 1위안 = %.2f원\n", get_cny_to_krw_rate());
    printf("\n국가별 면세 범위:\n");
    printf("- 미국: 200달러\n");
    printf("- 유럽연합: 약 %.0f유로 (150달러)\n", 150 * get_usd_to_krw_rate() / get_eur_to_krw_rate());
    printf("- 일본: 약 %.0f엔 (150달러)\n", 150 * get_usd_to_krw_rate() / get_jpy_to_krw_rate());
	printf("- 중국: 약 %.0f위안 (150달러)\n", 150 * get_usd_to_krw_rate() / get_cny_to_krw_rate());
    printf("- 한국(기준): 150달러\n");
    printf("(※ 배송료 포함 금액 기준이며, 전자제품/주류/식품은 별도 과세될 수 있음)\n");
}