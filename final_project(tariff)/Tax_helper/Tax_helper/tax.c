#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tax.h"
#include "exchange_api.h"

double calculate_tax(double price, const char* rate_str) {
	//������ ���÷� 20%�� ����, �����δ� rate_str���� �����;� ��
    double rate = 0.2;//atof(rate_str);
    double total = price * rate;
    printf("�ش� ��ǰ�� û���Ǵ� ������ %f �� �Դϴ�.\n", total);
}

// Ʈ�� ��ȸ�ϸ� ī�װ�/���� ���
void show_tariff_by_category(TreeNode* root) {
    printf("ī�װ��� ����/�ΰ����� ���:\n");
    print_tree(root, 0);
}

// ������ ȯ��, �鼼 ���� ����
void show_country_rate_and_duty_free_info() {
    printf("\n�ֿ� 4���� ȯ�� ����\n");
	printf("- �̱�: 1�޷� = %.2f��\n", get_usd_to_krw_rate());
	printf("- ��������: 1���� = %.2f��\n", get_eur_to_krw_rate());
	printf("- �Ϻ�: 1�� = %.2f��\n", get_jpy_to_krw_rate());
	printf("- �߱�: 1���� = %.2f��\n", get_cny_to_krw_rate());
    printf("\n������ �鼼 ����:\n");
    printf("- �̱�: 200�޷�\n");
    printf("- ��������: �� %.0f���� (150�޷�)\n", 150 * get_usd_to_krw_rate() / get_eur_to_krw_rate());
    printf("- �Ϻ�: �� %.0f�� (150�޷�)\n", 150 * get_usd_to_krw_rate() / get_jpy_to_krw_rate());
	printf("- �߱�: �� %.0f���� (150�޷�)\n", 150 * get_usd_to_krw_rate() / get_cny_to_krw_rate());
    printf("- �ѱ�(����): 150�޷�\n");
    printf("(�� ��۷� ���� �ݾ� �����̸�, ������ǰ/�ַ�/��ǰ�� ���� ������ �� ����)\n");
}