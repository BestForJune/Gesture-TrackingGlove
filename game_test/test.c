//
// Created by zheng on 10/23/2020.
//

#include "game_arr.h"
#include <stdio.h>
void bin(uint8_t n);

int main() {
    uint8_t nb = 2;
//    uint8_t new;
    for (int i = 0; i < 50; ++i) {
        nb = rand_gen(nb);

        bin(nb);
        printf("\n");

    }
}

void bin(uint8_t n)
{
    /* step 1 */
    if (n > 1)
        bin(n/2);

    /* step 2 */
    printf("%d", n % 2);
}
