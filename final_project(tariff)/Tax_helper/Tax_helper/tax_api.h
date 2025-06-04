#pragma once
#ifndef TAX_API_H
#define TAX_API_H

// °ü¼¼Ã» UNIPASS °ü¼¼À² Á¶È¸ API
char* get_tariff_rate_from_api(const char* hs_code);
void fetch_tariff_from_api(const char* hs_code);
#endif