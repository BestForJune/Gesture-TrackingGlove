//
// Created by zheng on 10/7/2020.
//
#include <iostream>
#include "score_board.h"
#include <unistd.h>
#include <cstdlib>

using namespace std;


string gen_random(const int len) {

    string tmp_s;
    static const char alphanum[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";

    srand( (unsigned) time(NULL) * getpid());

    for (int i = 0; i < len; ++i)
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];


    return tmp_s;

}

int main() {
    char ran[MAX_NAME_LEN + 1];
    score_board board;
    board.update("Zhengsen", 8, 1);
    board.update("Zhengsen", 8, 2);
    board.update("Zhengsen", 8, 3);
    board.update("Zhengsen", 8, 1);
    board.update("Zhengsen", 8, 2);
    board.update("Zhengsen", 8, 2);
    board.update("Yanjun", 6, 1);
    board.update("Yanjun", 6, 2);
    board.update("Yanjun", 6, 3);
    board.update("Yanjun", 6, 4);
    board.update("Tianze", 6, 2);
    board.update("Yanjun", 6, 5);
    board.update("Yanjun", 6, 5);
    for(int lcv = 0; lcv < 100; lcv++) {
        int rand_len = rand() % 10 + 1;
        short rand_score = rand() % 256;
        strcpy(ran, gen_random(rand_len).c_str());

        board.update(ran, rand_len, rand_score);
        board.print_board();
        cout << lcv;
        cout << '\n';
    }



}


