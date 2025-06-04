// tax_calc.h

#ifndef TAX_CALC_H
#define TAX_CALC_H

#define MAX_NAME_LEN 128

// CSV에서 읽어들인 한 행의 데이터를 담는 구조체
typedef struct {
    char taxType[MAX_NAME_LEN];   // 세금종류(=카테고리), 예: "의류_및_패션잡화"
    char itemName[MAX_NAME_LEN];  // 품목명, 예: "가방(200만원 이하)"
    double tariffRate;            // 관세율(%) 예: 8.0
    double vatRate;               // 부가세율(%) 예: 10.0
    double specialRate;           // 특소세율(%) 예: 20.0
} TaxItem;

// tax_items.csv를 로드해서 내부에 저장
void loadTaxItems(const char* csvPath);

// 메뉴 3번 “세금 계산”을 처리하는 함수
void handleTaxCalculation(void);

#endif // TAX_CALC_H

