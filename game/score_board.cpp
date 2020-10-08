//
// Created by zheng on 10/7/2020.
//

#include "score_board.h"
#include <iostream> //debug
#include <string>


player::player() {
    for(char & lcv : name) {
        lcv = '-';
    }
    id = 0;
    name[MAX_NAME_LEN] = '\0';
    name_len = MAX_NAME_LEN;
    score = 0;
}

void player::new_player(const char *input_name, short len, short input_score) {
    short lcv = 0;
    for(; lcv < len; lcv++) {
        name[lcv] = input_name[lcv];
    }
    name[lcv] = '\0';
    name_len = len;
    score = input_score;
    id = calc_id(input_name, name_len);
}

unsigned int calc_id(const char *name, short len) {
    unsigned int result = 0;
    unsigned int temp;

    for(short i = 0; i < len; i++) {
        temp = (unsigned char)name[i];
        result ^= temp << (i % 4);
    }
    return result;
}

void player::update_score(short new_score) {
    if (new_score > score) {
        score = new_score;
    }
}


void player::to_string(char * buf) {
    sprintf(buf,"%s: %d id: %d\n", name, score, id); //possible useless \n
}

void player::copy(player input_player) {
    strcpy_s(name, MAX_NAME_LEN, input_player.name);
    name_len = input_player.name_len;
    score = input_player.score;
    id = input_player.id;
}


score_board::score_board() {
    num_player = 0;
}

// debug
void score_board::print_board() {
    for(player each : players) {
        each.to_string(buf);
        std::cout << buf;
    }
}

void score_board::update(const char *input_name, short len, short input_score) {
    short index = 0;
    bool found = false;
    unsigned int new_id = calc_id(input_name, len);
    while (index < num_player) {
        if (new_id == players[index].get_id()) {
            found = true;
            break;
        }
        index++;
    }
    if(found) {
        players[index].update_score(input_score);
    }
    else {
        //if players are full
        if (num_player == MAX_PLAYER) {
            index = 0;
            while (index < MAX_PLAYER && input_score < players[index].get_score()) {
                index++;
            }
            if (index < MAX_PLAYER) {
                shift_player(index);
                players[index].new_player(input_name, len, input_score);
            }
            // or discard player data
        }
        else {
            num_player++;
            players[num_player - 1].new_player(input_name, len, input_score);
        }
    }

}

void score_board::shift_player(short from) {
    for (short lcv = MAX_PLAYER; lcv > from + 1; lcv--){
        players[lcv].copy(players[lcv - 1]);
    }
}




