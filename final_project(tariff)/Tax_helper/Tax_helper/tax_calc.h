// tax_calc.h

#ifndef TAX_CALC_H
#define TAX_CALC_H

#define MAX_NAME_LEN 128

// CSV���� �о���� �� ���� �����͸� ��� ����ü
typedef struct {
    char taxType[MAX_NAME_LEN];   // ��������(=ī�װ�), ��: "�Ƿ�_��_�м���ȭ"
    char itemName[MAX_NAME_LEN];  // ǰ���, ��: "����(200���� ����)"
    double tariffRate;            // ������(%) ��: 8.0
    double vatRate;               // �ΰ�����(%) ��: 10.0
    double specialRate;           // Ư�Ҽ���(%) ��: 20.0
} TaxItem;

// tax_items.csv�� �ε��ؼ� ���ο� ����
void loadTaxItems(const char* csvPath);

// �޴� 3�� ������ ��ꡱ�� ó���ϴ� �Լ�
void handleTaxCalculation(void);

#endif // TAX_CALC_H

