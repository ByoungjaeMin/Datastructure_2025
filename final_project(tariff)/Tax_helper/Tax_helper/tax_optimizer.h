#pragma once
#ifndef TAX_OPTIMIZER_H
#define TAX_OPTIMIZER_H

#define MAX_ITEMS 100
#define MAX_BOXES 20
#define DUTY_FREE_LIMIT 150.0 // ����� �ѵ� (USD)

typedef struct {
    char name[50];
    double price;      // ���� ���� (��ȭ ����)
    int quantity;      // ����
    char hs_code[10];  // ����û �ڵ�
    double duty_rate;  // ������ (0.08 ��)
} Item;

typedef struct {
    Item* items[MAX_ITEMS];
    int count;
    double total_price;
    double total_tax;
} Box;

// �ܺο��� ���� ������ ����
extern Item item_list[MAX_ITEMS];
extern int item_count;
extern Box boxes[MAX_BOXES];
extern int box_count;

// ����ȭ �˰��� �Լ�
void build_duty_free_boxes();
void build_duty_boxes();
void print_boxes();
void reset_optimizer();

#endif