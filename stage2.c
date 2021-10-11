#include "ass2.h"

void do_stage2(board_t board, nxt_act_t *nxt_act) {
    printf("stage 2");
    for (int i = 0; i < 10; i++) {
        do_stage1(board, nxt_act);
    }
}