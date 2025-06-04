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
#include "tree_from_csv.h"

// 여기에 새로 분리된 tax_calc 모듈 헤더를 추가합니다.
#include "tax_calc.h"

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
    // 1) 트리를 CSV로부터 빌드 (기존 대로)
    TreeNode* tariff_tree_root = build_tree_from_csv("tariff.csv");
    if (!tariff_tree_root) {
        printf("트리 생성 실패\n");
        return 1;
    }

    // 2) tax_items.csv를 한 번만 로드합니다.
    //    프로젝트 구조에 따라 경로를 조정하세요.
    loadTaxItems("tax_items.csv");
    // tax_calc 모듈 내부에서 로드 실패 시 stderr 출력하도록 되어 있습니다.

    char choice;
    do {
        show_menu();
        scanf("%c", &choice);
        if (getchar() == '\n') {
            fflush(stdin);
        }

        switch (choice) {
        case '1': {
            // 기존에 구현된 트리 기반 카테고리 조회 로직 그대로 유지
            char item_name[128];
            print_tree(tariff_tree_root, 0);
            printf("조회할 품목명을 입력하세요 (예: 의류/수영복/속옷): ");
            scanf("%s", item_name);
            if (getchar() == '\n') fflush(stdin);

            TreeNode* found = search_tree(tariff_tree_root, item_name);
            if (found) {
                printf("품목 '%s'의 세금 정보: %s\n", found->name, found->data);
            }
            else {
                printf("해당 품목을 찾을 수 없습니다.\n");
            }
            break;
        }
        case '2':
            // TODO: 국가별 면세 한도 정보 출력
            show_country_duty_free_info();
            break;

        case '3':
            // 3번 메뉴—별도의 모듈(tax_calc.c/h)에서 처리
            handleTaxCalculation();
            break;

        case '4': {
            // 예시: 최적화 알고리즘 실행
            printf("[4] 최적 관부가세 및 박스 묶음 계산 실행\n");

            // (예시용 환율 조회 — 실제 구현 시 캐시 적용 가능)
            double rate = get_usd_to_krw_rate();
            printf("현재 환율: USD 1 = %.2f KRW\n", rate);

            // 관세율 API는 각 아이템에 개별 적용
            // 예: item_list[i].duty_rate = atof(get_tariff_rate_from_api(item_list[i].hs_code));

            reset_optimizer();
            build_duty_free_boxes();
            build_duty_boxes();
            print_boxes();
            break;
        }
        case '5':
            printf("프로그램을 종료합니다.\n");
            break;

        default:
            printf("잘못된 입력입니다. 다시 선택해주세요.\n");
            break;
        }
    } while (choice != '5');

    return 0;
}
