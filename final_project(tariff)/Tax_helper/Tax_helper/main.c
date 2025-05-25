// main.c - 전체 프로그램 흐름 제어
#include <stdio.h>
#include "tax.h"
#include "tax_api.h"
#include "exchange_api.h"
#include "tax_optimizer.h"

void show_menu() {
    printf("\n===== 해외직구 관부가세 계산기 CLI =====\n");
    printf("1. 카테고리별 관세/부가세율 보기\n");
    printf("2. 국가별 면세 범위 보기\n");
    printf("3. 세금 계산\n");
    printf("4. 관부가세/배송비 최적화 계산기\n");
    printf("5. 종료\n");
    printf("선택: ");
}

int main() {
    int choice;
    do {
        show_menu();
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            // TODO: 트리 구조에서 카테고리별 세율 보기
            printf("[1] 카테고리별 관세/부가세율 보기 (구현 예정)\n");
            break;
        case 2:
            // TODO: 국가별 면세 한도 정보 출력
            show_country_duty_free_info();
            break;
        case 3:
            // TODO: 상품 가격/수량 입력 후 세금 계산
            printf("[3] 세금 계산 (구현 예정)\n");
            break;
        case 4:
            // 환율 및 관세율 API → 최적화 알고리즘 실행
            printf("[4] 최적 관부가세 및 박스 묶음 계산 실행\n");

            // (예시용 환율 조회 — 실제 구현 시 캐시 적용 가능)
            double rate = get_usd_to_krw_rate();
            printf("현재 환율: USD 1 = %.2f KRW\n", rate);

            // 관세율 API는 각 item_list[i]에 적용해야 함 (개별적으로)
            // for (int i = 0; i < item_count; i++) {
            //     item_list[i].duty_rate = atof(get_tariff_rate_from_api(item_list[i].hs_code));
            // }

            reset_optimizer();
            build_duty_free_boxes();
            build_duty_boxes();
            print_boxes();
            break;
        case 5:
            printf("프로그램을 종료합니다.\n");
            break;
        default:
            printf("잘못된 입력입니다. 다시 선택해주세요.\n");
        }
    } while (choice != 5);

    return 0;
}
