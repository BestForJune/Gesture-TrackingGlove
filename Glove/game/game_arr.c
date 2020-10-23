//
// Created by zheng on 10/23/2020.
//

#include "game_arr.h"
#include <stdint.h>


uint8_t rand_gen(uint8_t seed) {
    seed >> 1 & 0x1;
    seed >> 2 & 0x1;
    return (seed << 3) ^ (seed >> 5);
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
        line_init(arr->lines[i]);
    }
    arr->head=0;
}


