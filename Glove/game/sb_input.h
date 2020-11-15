//
// Created by zheng on 11/15/2020.
//

#ifndef GAME_SB_INPUT_H
#define GAME_SB_INPUT_H
#include "score_board.h"
#include <stdint.h>
#include <stdio.h>
#include "MY_ILI9341.h"
#include "debounce.h"
extern scoreboard board;
//extern uint8_t screen[ILI9341_WIDTH *2];
void game_end(short score_earned);
int input_name(char* name_buf);

#endif //GAME_SB_INPUT_H
