//
// Created by zheng on 11/3/2020.
//

#include "debounce.h"

void init_debounce(uint8_t* history, uint8_t* bent_ref) {
    for (int i = 0; i < HISTORY_NUM; ++i) {
        history[i] = 0;
        bent_ref[i] = 0;
    }
    bent_ref[0] = 1;
}

void update_debounce(uint8_t* history, uint8_t input) {
    for (int i = HISTORY_NUM - 1; i > 0; --i) {
        history[i] = history[i-1];
    }
    history[0] = input;
}


uint8_t match_bent(uint8_t* history, uint8_t* bent_ref) {
    uint8_t match = 1;
    for (int i = 0; i < HISTORY_NUM; ++i) {
        if (history[i] != bent_ref[i]) {
            match = 0;
            break;
        }
    }
    return match;
}
