// tax_optimizer.c - 관부가세 최적화 알고리즘

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "tax_optimizer.h"

Item item_list[MAX_ITEMS];
int item_count = 0;
Box boxes[MAX_BOXES];
int box_count = 0;

void build_duty_free_boxes() {
    int best_mask = 0;
    double best_value = 0.0;

    for (int mask = 0; mask < (1 << item_count); ++mask) {
        double total = 0.0;
        for (int i = 0; i < item_count; ++i) {
            if (mask & (1 << i)) {
                total += item_list[i].price * item_list[i].quantity;
            }
        }
        if (total <= DUTY_FREE_LIMIT && total > best_value) {
            best_value = total;
            best_mask = mask;
        }
    }

    Box* b = &boxes[box_count++];
    b->total_price = 0.0;
    b->total_tax = 0.0;
    b->count = 0;
    for (int i = 0; i < item_count; ++i) {
        if (best_mask & (1 << i)) {
            b->items[b->count++] = &item_list[i];
            b->total_price += item_list[i].price * item_list[i].quantity;
        }
    }
}

void build_duty_boxes() {
    Item* remaining[MAX_ITEMS];
    int r_count = 0;
    for (int i = 0; i < item_count; ++i) {
        int used = 0;
        for (int b = 0; b < box_count; ++b) {
            for (int j = 0; j < boxes[b].count; ++j) {
                if (boxes[b].items[j] == &item_list[i]) used = 1;
            }
        }
        if (!used) remaining[r_count++] = &item_list[i];
    }

    for (int i = 0; i < r_count - 1; ++i) {
        for (int j = i + 1; j < r_count; ++j) {
            double val_i = remaining[i]->duty_rate * remaining[i]->price;
            double val_j = remaining[j]->duty_rate * remaining[j]->price;
            if (val_i < val_j) {
                Item* tmp = remaining[i];
                remaining[i] = remaining[j];
                remaining[j] = tmp;
            }
        }
    }

    for (int i = 0; i < r_count; ++i) {
        Box* b = &boxes[box_count++];
        b->count = 1;
        b->items[0] = remaining[i];
        b->total_price = remaining[i]->price * remaining[i]->quantity;
        b->total_tax = b->total_price * remaining[i]->duty_rate;
    }
}

void print_boxes() {
    for (int i = 0; i < box_count; ++i) {
        printf("\n📦 Box %d:\n", i + 1);
        for (int j = 0; j < boxes[i].count; ++j) {
            Item* it = boxes[i].items[j];
            printf("- %s (x%d): %.2f each, duty %.2f\n", it->name, it->quantity, it->price, it->duty_rate);
        }
        printf("총 가격: %.2f\n", boxes[i].total_price);
        printf("추정 관세: %.2f\n", boxes[i].total_tax);
    }
}

void reset_optimizer() {
    box_count = 0;
}
