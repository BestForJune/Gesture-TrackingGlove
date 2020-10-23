//
// Created by zheng on 10/23/2020.
//

#ifndef GESTURE_TRACKINGGLOVE_GAME_ARR_H
#define GESTURE_TRACKINGGLOVE_GAME_ARR_H
#include <stdint.h>
#define MAX_LINE 16

uint8_t rand_gen(uint8_t seed);

typedef struct LINE{
    uint8_t pattern[5]; // finger expected patter
    uint8_t y_pos;  // y position of this line
    uint8_t next; // next node of the linked list. MAX_LINE = null
} line;

void line_init(line*);


typedef struct GAME_ARR{
    line lines[MAX_LINE]; // all lines available for the game
    uint8_t head; // MAX_LINE = null
} game_arr;

void game_arr_init(game_arr *);



#endif //GESTURE_TRACKINGGLOVE_GAME_ARR_H
