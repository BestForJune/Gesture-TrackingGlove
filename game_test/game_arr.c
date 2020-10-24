//
// Created by zheng on 10/23/2020.
//

#include "game_arr.h"
#include <stdint.h>


void rand_gen(line* in_l, uint8_t seed) {
    uint8_t n = 5;
    uint8_t new = (seed << 3) ^ (seed >> 5) ^ (n << seed);
    in_l->seed = new;
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


void game_arr_init(game_arr * arr, uint8_t seed_in) {
    for (int i = 0; i < MAX_LINE; ++i) {
        line_init(&arr->lines[i]);
    }
    arr->head = MAX_LINE;
    arr->lines[arr->head].seed = seed_in;
    new_line(arr);
}

void new_line(game_arr * arr) {
    uint8_t old_head = arr->head;
    if(arr->head + 1 >= MAX_LINE) {
        arr->head = 0;
    } else {
        arr->head = old_head + 1;
    }
    rand_gen(&arr->lines[arr->head], arr->lines[old_head].seed);
    arr->lines[arr->head].next = old_head;
    arr->lines[arr->head].y_pos = 9;
}

void arr_update(game_arr * arr) {
    //condition to generate a new line
    if(arr->lines[arr->head].y_pos >= 40) {
        new_line(arr);
    }
    uint8_t ptr = arr->head; // linked list traverse pointer
    while(ptr != MAX_LINE) {
        arr->lines[ptr].y_pos++;
        if(arr->lines[arr->lines[ptr].next].y_pos >= HEIGHT) {
            arr->lines[ptr].next = MAX_LINE;
        }

        ptr = arr->lines[ptr].next;
    }
}


