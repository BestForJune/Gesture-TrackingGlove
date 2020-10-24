//
// Created by zheng on 10/23/2020.
//

#ifndef GESTURE_TRACKINGGLOVE_GAME_ARR_H
#define GESTURE_TRACKINGGLOVE_GAME_ARR_H
#include <stdint.h>
#define MAX_LINE 16
#define HEIGHT 320



typedef struct LINE{
    uint8_t pattern[5]; // finger expected patter
    uint16_t y_pos;  // y position of this line
    uint8_t next; // next node of the linked list. MAX_LINE = null
    uint8_t seed;
} line;

void line_init(line*);
void rand_gen(line* in_l, uint8_t seed);

typedef struct GAME_ARR{
    line lines[MAX_LINE + 1]; // all lines available for the game. Plus one dummy line
    uint8_t head;
} game_arr;

void game_arr_init(game_arr * arr, uint8_t seed_in);
void new_line(game_arr * arr);
void arr_update(game_arr * arr);


#endif //GESTURE_TRACKINGGLOVE_GAME_ARR_H
