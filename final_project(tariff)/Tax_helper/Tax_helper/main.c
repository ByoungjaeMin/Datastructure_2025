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
        printf("오류: 트리 생성 실패. 'tariff.csv' 파일이 올바른 위치에 있는지 확인하세요.\n");
        return 1;
    }

    // 2) tax_items.csv를 한 번만 로드합니다.
    //    프로젝트 구조에 따라 경로를 조정하세요.
    loadTaxItems("tax_items.csv");
    loadTaxItemsOpt("tax_items.csv");

    char choice;
    do {
        show_menu();
        if (scanf(" %c", &choice) != 1) {
            printf("잘못된 입력입니다. 다시 시도하세요.\n");
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n');

        switch (choice) {
        case '1': {
            char item_name[128];
            print_tree(tariff_tree_root, 0);
            printf("조회할 품목명을 입력하세요 (예: 의류/수영복/속옷): ");
            if (fgets(item_name, sizeof(item_name), stdin) == NULL) {
                printf("입력 오류\n");
                break;
            }
            item_name[strcspn(item_name, "\r\n")] = '\0';

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
            show_country_rate_and_duty_free_info();
            break;

        case '3':
            handleTaxCalculation();
            break;

        case '4': {
            printf("[4] 관부가세/배송비 최적화 계산기\n");

            char optChoice_char;
            do {
                printf("\n--- 최적화 계산기 메뉴 ---\n");
                printf("1. 상품 등록\n");
                printf("2. 관세 최적화 계산 및 박스 묶음 보기\n");
                printf("3. 초기화 (등록된 상품 제거)\n");
                printf("4. 이전 메뉴로 돌아가기\n");
                printf("선택: ");
                if (scanf(" %c", &optChoice_char) != 1) {
                    printf("잘못된 입력입니다. 숫자를 입력해주세요.\n");
                    while (getchar() != '\n');
                    continue;
                }
                while (getchar() != '\n');

                switch (optChoice_char) {
                case '1':
                    handleTaxCalculation_Optmize();
                    break;
                case '2':
                    if (item_count == 0) {
                        printf("최적화할 상품이 없습니다. 상품을 먼저 등록해주세요.\\n");
                    }
                    else {
                        selectcur();
                        build_duty_free_boxes();
                        build_duty_boxes();
                        print_boxes();
                    }
                    break;
                case '3':
                    reset_optimizer();
                    printf("최적화 계산기 상품 목록이 초기화되었습니다.\\n");
                    break;
                case '4':
                    printf("이전 메뉴로 돌아갑니다.\n");
                    break;
                default:
                    printf("잘못된 선택입니다.\n");
                    break;
                }
            } while (optChoice_char != '4');
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