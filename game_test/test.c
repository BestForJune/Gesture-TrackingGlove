//
// Created by zheng on 10/23/2020.
//

//#include "game_arr.h"
//#include <stdio.h>
//void bin(uint8_t n);
//
//int game_score;
//
//int main() {
//    uint8_t seed = 4;
//    game_arr arr;
//    game_arr_init(&arr, seed);
//
//    for (int i = 0; i < 200; ++i) {
//        arr_update(&arr);
//
//        printf("iteration %d \n", i);
//        printf("head %d \n", arr.head);
//        uint8_t ptr = arr.head; // linked list traverse pointer
//        while(ptr != MAX_LINE) {
//            printf("%d: ", ptr);
//            for (int j = 0; j < 5; ++j) {
//                printf("%d", arr.lines[ptr].pattern[j]);
//            }
//            ptr = arr.lines[ptr].next;
//            printf("   y_pos: %d     next:%d\n", arr.lines[ptr].y_pos, arr.lines[ptr].next);
//        }
//        printf("\n\n");
//    }
//
//    // for (int i = 0; i < 1200; ++i) {
//    //     arr_update(&arr);
//
//    //     printf("iteration %d \n", i);
//    //     printf("head %d \n", arr.head);
//    //     uint8_t ptr = arr.head; // linked list traverse pointer
//    //     while(ptr != MAX_LINE) {
//    //         printf("%d: ", ptr);
//    //         for (int j = 0; j < 5; ++j) {
//    //             printf("%d", arr.lines[ptr].pattern[j]);
//    //         }
//    //         ptr = arr.lines[ptr].next;
//    //         printf("   y_pos: %d     next:%d\n", arr.lines[ptr].y_pos, arr.lines[ptr].next);
//    //     }
//    //     printf("\n\n");
//    // }
//
//
//    return 0;
//}
//
//void bin(uint8_t n)
//{
//    /* step 1 */
//    if (n > 1)
//        bin(n/2);
//
//    /* step 2 */
//    printf("%d", n % 2);
//}

#include "sb_input.h"

int main() {
    game_end(20);
}
