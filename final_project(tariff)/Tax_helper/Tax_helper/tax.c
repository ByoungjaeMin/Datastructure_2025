#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tax.h"

double calculate_tax(double price, const char* rate_str) {
    double rate = atof(rate_str);
    return price * rate;
}

// Ʈ�� ��ȸ�ϸ� ī�װ�/���� ���
void show_tariff_by_category(TreeNode* root) {
    printf("ī�װ��� ����/�ΰ����� ���:\n");
    print_tree(root, 0);
}

// ������ �鼼 ���� ����
void show_country_duty_free_info() {
    printf("������ �鼼 ����:\n");
    printf("- �̱�: 200�޷�\n");
    printf("- �Ϻ�: 10,000�� (�� 90�޷�)\n");
    printf("- ����: 150���� (�� 160�޷�)\n");
    printf("- �ѱ�(����): 150�޷�\n");
    printf("(�� ��۷� ���� �ݾ� �����̸�, ������ǰ/�ַ�/��ǰ�� ���� ������ �� ����)\n");
}
