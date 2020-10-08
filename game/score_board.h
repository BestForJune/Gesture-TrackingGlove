//
// Created by zheng on 10/7/2020.
//

#ifndef GESTURE_TRACKINGGLOVE_SCORE_BOARD_H
#define GESTURE_TRACKINGGLOVE_SCORE_BOARD_H
#define MAX_NAME_LEN 10 // maximum number of character
#define MAX_PLAYER 10 // maximum number of players in
#define MAX_LINE 20 // maximum number of character in print buffer
#include <string.h>


class player {
private:
    char name[MAX_NAME_LEN + 1];
    short name_len; //length of the user name
    short score;
    unsigned int id; //faster to look for the player

public:
    player();
    void new_player(const char* input_name, short len, short input_score);
    void update_score(short new_score);
    void to_string(char * buf);
    unsigned int get_id() {return  id;};
    short get_score() {return score;};
    void copy(player input_player);
};

unsigned int calc_id(const char *name, short len);

class score_board {
private:
    player players[MAX_PLAYER];
    short num_player; //number of players already on the score board
    char buf[MAX_LINE];

    void shift_player(short from, short to);
    void swap_player(short which);

public:
    score_board();
    void print_board();
    void update(const char* input_name, short len, short input_score);
};

#endif //GESTURE_TRACKINGGLOVE_SCORE_BOARD_H
