//
// Created by zheng on 10/7/2020.
//
#include <iostream>
#include "score_board.h"

int main() {
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


    board.print_board();


}

