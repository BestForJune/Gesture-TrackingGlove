//
// Created by zheng on 10/23/2020.
//

#include "game_arr.h"
#include <stdint.h>


void rand_gen(line* in_l, uint8_t seed) {
    uint8_t n = 5;
    uint8_t new = (seed << 3) ^ (seed >> 5) ^ (n << seed);
    in_l->seed = new;
    in_l->next = MAX_LINE;
    in_l->y_pos = 0;
    for (int i = 0; i < 5; ++i) {
        in_l->pattern[i] = new >> i & 0x1;
    }
}

void line_init(line* input_l) {
    for (int i = 0; i < 5; ++i) {
        input_l -> pattern[i] = 0;
    }
    input_l -> y_pos = 0;
    input_l -> next = MAX_LINE;
}


void game_arr_init(game_arr * arr) {
    for (int i = 0; i < MAX_LINE; ++i) {
        line_init(&arr->lines[i]);
    }
    arr->head=0;
}


