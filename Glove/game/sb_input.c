//
// Created by zheng on 11/15/2020.
//

#include "sb_input.h"
#define NAME_POS 180
#define INSTRUCTION_POS 220
#define INSTRUCTION_DIFF 20

void game_end(short score_earned) { //call after the game has ended
	char score_buf[10];
    char name_buf[MAX_NAME_LEN+1];
    sprintf(score_buf, "%d", score_earned);
	ILI9341_printText("Your score is:", 40, 30, COLOR_GREEN, COLOR_RED, 2);
	ILI9341_printText(score_buf, 50, 100, COLOR_GREEN, COLOR_GREEN, 5);
	ILI9341_printText("Please enter your name", 40, 150, COLOR_GREEN, COLOR_RED, 1);

	ILI9341_printText("Thumb to confirm character", 30, INSTRUCTION_POS, COLOR_GREEN, COLOR_RED, 1);
	ILI9341_printText("Index for next character", 30, INSTRUCTION_POS + INSTRUCTION_DIFF, COLOR_GREEN, COLOR_RED, 1);
	ILI9341_printText("Middle for previous character", 30, INSTRUCTION_POS + INSTRUCTION_DIFF * 2, COLOR_GREEN, COLOR_RED, 1);
	ILI9341_printText("Ring to delete", 30, INSTRUCTION_POS + INSTRUCTION_DIFF * 3, COLOR_GREEN, COLOR_RED, 1);
	ILI9341_printText("Little to finish", 30, INSTRUCTION_POS + INSTRUCTION_DIFF * 4, COLOR_GREEN, COLOR_RED, 1);

    short name_len = input_name(name_buf);
    scoreboard_update(&board, name_buf, name_len, score_earned);
}

int input_name(char* name_buf) {
//    uint8_t done = 0; //if the name inputting process has done
    uint8_t counter = 0; //number of characters has been inputted
//    uint8_t thumb; //status of thumb
//    uint8_t index; //status of index finger
//    uint8_t middle; //status of middle finger
//    uint8_t ring; // status of ring finger
//    uint8_t little; //status of little finger

    uint8_t history_index_local[HISTORY_NUM];
    uint8_t history_thumb_local[HISTORY_NUM];
    uint8_t history_middle_local[HISTORY_NUM];
    uint8_t history_ring_local[HISTORY_NUM];
    uint8_t history_little_local[HISTORY_NUM];
    uint8_t bent_ref_local[HISTORY_NUM];
    init_debounce(history_index_local, bent_ref_local);
    init_debounce(history_thumb_local, bent_ref_local);
    init_debounce(history_middle_local, bent_ref_local);
    init_debounce(history_ring_local, bent_ref_local);
    init_debounce(history_little_local, bent_ref_local);

    //print please input your name
    //print thumb to confirm character
    //print index for next character
    //print middle for previous character
    //print ring to delete
    //print little to finish
    for (int i = 0; i < MAX_NAME_LEN; ++i) {
        name_buf[i] = '-';
    }
    name_buf[MAX_NAME_LEN] = '\0';
    name_buf[0] = 'A';
    while (1) {
        //record debouncing
    	update_debounce(history_thumb_local, HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_0));
    	update_debounce(history_index_local, HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_1));
    	update_debounce(history_middle_local, HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_2));
		update_debounce(history_ring_local, HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_3));
		update_debounce(history_little_local, HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_5));

		ILI9341_printText(name_buf, 50, NAME_POS, COLOR_GREEN, COLOR_GREEN, 2);

        if(match_bent(history_little_local, bent_ref_local)) {
            name_buf[counter+1] = '\0';
            return counter+1;
        }
        if(match_bent(history_ring_local, bent_ref_local)) {
            if(counter == 0) {
                continue;
            }
            name_buf[counter] = '-';
            counter--;
            for(int i = NAME_POS; i < NAME_POS + 20; i++) {
				ILI9341_Fill_Black_Line(i);
			}
            continue;
        }

        if(counter < MAX_NAME_LEN - 1) {
            if (match_bent(history_thumb_local, bent_ref_local)) {
                counter++;
                name_buf[counter] = 'A';
                for(int i = NAME_POS; i < NAME_POS + 20; i++) {
					ILI9341_Fill_Black_Line(i);
				}
                continue;
            }
		}
        if (match_bent(history_index_local, bent_ref_local)) {
            if (name_buf[counter] == 'Z') {
                name_buf[counter] = 'A';
            } else {
                name_buf[counter]++;
            }
            for(int i = NAME_POS; i < NAME_POS + 20; i++) {
				ILI9341_Fill_Black_Line(i);
			}
            continue;
        }
        if(match_bent(history_middle_local, bent_ref_local)) {
            if (name_buf[counter] == 'A') {
                name_buf[counter] = 'Z';
            } else {
                name_buf[counter]--;
            }
            for(int i = NAME_POS; i < NAME_POS + 20; i++) {
				ILI9341_Fill_Black_Line(i);
			}
            continue;
        }

    }

}
