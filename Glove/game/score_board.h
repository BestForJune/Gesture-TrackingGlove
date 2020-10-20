//
// Created by 傅正森 on 2020/10/11.
//

#ifndef TEST_SCORE_BOARD_H
#define TEST_SCORE_BOARD_H

#define MAX_NAME_LEN 10 // maximum number of character
#define MAX_PLAYER 10 // maximum number of players in
#define MAX_LINE 20 // maximum number of character in print buffer
#include <stdio.h>
#include <string.h>

typedef struct PLAYER{
    char name[MAX_NAME_LEN + 1];
    short name_len; //length of the user name
    short score;
    unsigned int id; //faster to look for the player
} player;

void player_init(player* input_player);
void new_player(player* input_player, const char* input_name, short len, short input_score);
void update_score(player* input_player, short new_score);
void to_string(player* input_player, char * buf);
void player_copy(player* to_player, player* from_player);
unsigned int calc_id(const char *name, short len);

typedef struct SCOREBOARD {
    player players[MAX_PLAYER];
    short num_player; //number of players already on the score board
    char buf[MAX_LINE];
} scoreboard;

void scoreboard_init(scoreboard * input_sb);
void shift_player(scoreboard * input_sb, short from, short to);
void swap_player(scoreboard * input_sb, short which);
void print_board(scoreboard * input_sb);
void scoreboard_update(scoreboard * input_sb, const char* input_name, short len, short input_score);


#endif //TEST_SCORE_BOARD_H
