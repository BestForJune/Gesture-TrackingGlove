//
// Created by zheng on 11/15/2020.
//

#include "sb_input.h"

void game_end(short score_earned) { //call after the game has ended
    char name_buf[MAX_NAME_LEN+1];
    input_name(name_buf);
    printf("%s\n",name_buf);
}

void input_name(char* name_buf) {
//    uint8_t done = 0; //if the name inputting process has done
    uint8_t counter = 0; //number of characters has been inputted
    uint8_t thumb; //status of thumb
    uint8_t index; //status of index finger
    uint8_t middle; //status of middle finger
    uint8_t ring; // status of ring finger
    uint8_t little; //status of little finger
    //print please input your name
    //print thumb to confirm character
    //print index for next character
    //print middle for previous character
    //print ring to delete
    //print little to finish
    int input; //debug
    for (int i = 0; i < MAX_NAME_LEN; ++i) {
        name_buf[i] = '-';
    }
    name_buf[MAX_NAME_LEN] = '\0';
    name_buf[0] = 'A';
    while (1) {
        //record debouncing
        printf("%s\n", name_buf); //debug
        //debug-------------
        scanf_s("%d", &input);
        if(input == 1) {
            thumb = 1;
            index = 0;
            middle = 0;
            ring = 0;
            little = 0;
        } else if(input == 2){
            thumb = 0;
            index = 1;
            middle = 0;
            ring = 0;
            little = 0;
        } else if(input == 3){
            thumb = 0;
            index = 0;
            middle = 1;
            ring = 0;
            little = 0;
        } else if(input == 4){
            thumb = 0;
            index = 0;
            middle = 0;
            ring = 1;
            little = 0;
        } else if(input == 5){
            thumb = 0;
            index = 0;
            middle = 0;
            ring = 0;
            little = 1;
        }
        //debug-------------

        if(little) {
            name_buf[counter+1] = '\0';
            return;
        }
        if(ring) {
            if(counter == 0) {
                continue;
            }
            name_buf[counter] = '-';
            counter--;
            continue;
        }

        if(counter < MAX_NAME_LEN) {
            if (thumb) {
                counter++;
                name_buf[counter] = 'A';
                continue;
            }
        }
        if (index) {
            if (name_buf[counter] == 'Z') {
                name_buf[counter] = 'A';
            } else {
                name_buf[counter]++;
            }
        }
        if(middle) {
            if (name_buf[counter] == 'A') {
                name_buf[counter] = 'Z';
            } else {
                name_buf[counter]--;
            }
        }

    }

}
