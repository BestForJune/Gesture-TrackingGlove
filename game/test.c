//
// Created by 傅正森 on 2020/10/11.
//

#include "score_board.h"
#include <stdlib.h>

static char *rand_string(char *str, size_t size)
{
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    if (size) {
        --size;
        for (size_t n = 0; n < size; n++) {
            int key = rand() % (int) (sizeof charset - 1);
            str[n] = charset[key];
        }
        str[size] = '\0';
    }
    return str;
}

int main() {
    char ran_name[MAX_NAME_LEN + 1];
    short ran_len;
    short ran_score;

    scoreboard sb;
    scoreboard_init(&sb);
    scoreboard_update(&sb, "zhengsen", 8, 2);
    scoreboard_update(&sb, "zhengsen", 8, 3);
    scoreboard_update(&sb, "zhengsen", 8, 4);
    scoreboard_update(&sb, "Yanjun", 6, 5);
    scoreboard_update(&sb, "zhengsen", 8, 6);
    scoreboard_update(&sb, "Tianze", 6, 900);
    for (int i = 0; i < 10; ++i) {
        ran_len = rand() % MAX_NAME_LEN + 1;
        rand_string(&ran_name, ran_len);
        ran_score = rand() % 800;
        scoreboard_update(&sb, ran_name, ran_len, ran_score);
    }
    print_board(&sb);

}