//
// Created by zheng on 10/7/2020.
//

#include "score_board.h"
#include <iostream> //debug


player::player() {
    for(short lcv = 0; lcv < MAX_NAME_LEN; lcv++) {
        name[lcv] = '-';
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
    sprintf(buf,"%s: %d\n", name, score); //possible useless \n
}

void player::copy(player input_player) {
    strcpy(name, input_player.name);
    name_len = input_player.name_len;
    score = input_player.score;
    id = input_player.id;
}


score_board::score_board() {
    num_player = 0;
}

// debug
void score_board::print_board() {
    for(short lcv = 0; lcv < MAX_PLAYER; lcv++) {
        players[lcv].to_string(buf);
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
        swap_player(index);
    }
    else {
        //if players are full
        if (num_player == MAX_PLAYER) {
            index = 0;
            while (index < MAX_PLAYER && input_score < players[index].get_score()) {
                index++;
            }
            if (index < MAX_PLAYER) {
                shift_player(index, MAX_PLAYER - 1);
                players[index].new_player(input_name, len, input_score);
            }
            // or discard player data
        }
        else {
            num_player++;
            players[num_player - 1].new_player(input_name, len, input_score);
            swap_player(num_player - 1);
        }
    }

}

// shift players down by one
void score_board::shift_player(short from, short to) {
    for (short lcv = to; lcv > from; lcv--){
        players[lcv].copy(players[lcv - 1]);
    }
}

void score_board::swap_player(short which) {
    short index = 0;
    player temp;
    while (index < which) {
        if(players[index].get_score() < players[which].get_score()) {
            temp.copy(players[which]);
            shift_player(index, which);
            players[index].copy(temp);
            break;
        }
        index++;
    }
}




