//
// Created by 傅正森 on 2020/10/11.
//
#include "score_board.h"


void player_init(player* input_player) {
    for(short lcv = 0; lcv < MAX_NAME_LEN; lcv++) {
        input_player -> name[lcv] = '-';
    }
    input_player -> id = 0;
    input_player -> name[MAX_NAME_LEN] = '\0';
    input_player -> name_len = MAX_NAME_LEN;
    input_player -> score = 0;
}

void new_player(player* input_player, const char* input_name, short len, short input_score) {
    short lcv = 0;
    for(; lcv < len; lcv++) {
        input_player -> name[lcv] = input_name[lcv];
    }
    input_player -> name[lcv] = '\0';
    input_player -> name_len = len;
    input_player -> score = input_score;
    input_player -> id = calc_id(input_name, input_player -> name_len);
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

void update_score(player* input_player, short new_score) {
    if (new_score > input_player -> score) {
        input_player -> score = new_score;
    }
}

void to_string(player* input_player, char * buf) {
    sprintf(buf,"%s: %d\n", input_player -> name, input_player -> score); //possible useless \n
}

void player_copy(player* to_player, player* from_player) {
    strcpy(to_player->name, from_player->name);
    to_player->name_len = from_player->name_len;
    to_player->score = from_player->score;
    to_player->id = from_player->id;
}

void scoreboard_init(scoreboard * input_sb) {
    for (short i = 0; i < MAX_PLAYER; ++i) {
        player_init(&input_sb->players[i]);
    }
    input_sb->num_player = 0;
}

void shift_player(scoreboard * input_sb, short from, short to) {
    for (short lcv = to; lcv > from; lcv--){
        player_copy(&(input_sb->players[lcv]), &(input_sb->players[lcv-1]));
    }
}

void swap_player(scoreboard * input_sb, short which) {
    short index = 0;
    player temp;
    while (index < which) {
        if(input_sb->players[index].score < input_sb->players[which].score) {
            player_copy(&temp, &(input_sb->players[which]));
            shift_player(input_sb, index, which);
            player_copy(&(input_sb->players[index]), &temp);
            break;
        }
        index++;
    }
}

void print_board(scoreboard * input_sb) {
    for(short lcv = 0; lcv < MAX_PLAYER; lcv++) {
        to_string(&(input_sb->players[lcv]), input_sb->buf);
        printf(input_sb->buf);
    }
}

void scoreboard_update(scoreboard * input_sb, const char* input_name, short len, short input_score) {
    short index = 0;
    short found = 0;
    unsigned int new_id = calc_id(input_name, len);
    while (index < input_sb->num_player) {
        if (new_id == input_sb->players[index].id) {
            found = 1;
            break;
        }
        index++;
    }
    if(found) {
        update_score(&(input_sb->players[index]), input_score);
        swap_player(input_sb, index);
    }
    else {
        //if players are full
        if (input_sb->num_player == MAX_PLAYER) {
            index = 0;
            while (index < MAX_PLAYER && input_score < input_sb->players[index].score) {
                index++;
            }
            if (index < MAX_PLAYER) {
                shift_player(input_sb, index, MAX_PLAYER - 1);
                new_player(&(input_sb->players[index]), input_name, len, input_score);
            }
            // or discard player data
        }
        else {
            input_sb->num_player++;
            new_player(&(input_sb->players[input_sb->num_player - 1]), input_name, len, input_score);
            swap_player(input_sb, input_sb->num_player - 1);
        }
    }
}





