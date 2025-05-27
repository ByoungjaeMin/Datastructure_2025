#define _CRT_SECURE_NO_WARNINGS
#define CURL_STATICLIB
#pragma comment(lib, "libcurld.lib")
#pragma comment(lib, "wldap32.lib")
#pragma comment(lib, "ws2_32.lib")

#include <stdio.h>
#include <string.h>
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

    char choice;
    do {
        show_menu();
        scanf("%c", &choice);
        if (getchar() == "\n") {
            fflush(stdin);
        }

        switch (choice) {
        case 49:
            // TODO: 트리 구조에서 카테고리별 세율 보기
            //show_tariff_by_category(); //상품 목록을 트리로 제작 후 구현 가능
            break;
        case 50:
            // TODO: 국가별 면세 한도 정보 출력
            show_country_duty_free_info();
            break;
        case 51:
            // TODO: 상품 가격/수량 입력 후 세금 계산
            Item* item;
            char name[50];
            //구동확인용 예시, 나중에 item_list 완성시 연동
            int quantity;
			Item item_list[][50] = {
				{"전자제품", 10000},
				{"의류", 6000},
				{"화장품",7000},
				{"식품", 4000},
				{"도서", 8000},
				{"주류", 12000},
				{"가전제품", 14000},
				{"악세서리", 20000},
				{"스포츠용품", 18000},
				{"기타", 3000}
			};
            int item_count = 10;
            printf("상품명과 수량을 입력해주세요: ");
            scanf("%s %d", &name, &quantity);
            if (getchar() == "\n") {
                fflush(stdin);
            }
            for (int i = 0; i < item_count; i++) {
				if (strcmp(item_list[i], name) == 0) {
					item = &item_list[i];
                    double price = item->price;
                    calculate_tax(price * quantity, 2/*get_tariff_rate_from_api(item_list[i].hs_code)*/);
					break;
                    }
                if (i == item_count - 1) {
                    printf("해당 상품이 목록에 없습니다.\n\n");
                    break;
				}
			}
            break;
        case 52:
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
        case 53:
            printf("프로그램을 종료합니다.\n");
            break;
        default:
            printf("잘못된 입력입니다. 다시 선택해주세요.\n");
        }
    } while (choice != 53);

    return 0;
}
