//
// Created by zheng on 11/3/2020.
//

#ifndef DISPLAY_DEBOUNCE_H
#define DISPLAY_DEBOUNCE_H
#include <stdint.h>
#define HISTORY_NUM 3
uint8_t history_index[HISTORY_NUM];
uint8_t history_thumb[HISTORY_NUM];
uint8_t bent_ref[HISTORY_NUM];

void init_debounce(uint8_t* history, uint8_t* bent_ref);
void update_debounce(uint8_t* history, uint8_t input);
uint8_t match_bent(uint8_t* history, uint8_t* bent_ref);

#endif //DISPLAY_DEBOUNCE_H
