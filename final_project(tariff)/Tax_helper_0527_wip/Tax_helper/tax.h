#pragma once
#ifndef TAX_H
#define TAX_H

#include "tree.h"

double calculate_tax(double price, const char* rate_str);
void show_tariff_by_category(TreeNode* root);
void show_country_duty_free_info();

#endif