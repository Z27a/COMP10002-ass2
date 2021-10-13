#include "ass2.h"

int
main(int argc, char *argv[]) {
    //TODO: Add function separator lines after i've finished

    // YOUR IMPLEMENTATION OF STAGES 0-2
    board_t board;
    nxt_act_t next;
    /* Set starting values */
    next.num_turns = 1;
    next.prev_turn = '\0';
    next.is_stage1 = FALSE;

    do_stage0(board, &next);

    if (next.is_stage1) {
        do_stage1(board, &next);
    } else {
        /* Stage 2 */
        for (int i = 0; i < COMP_ACTIONS; i++) {
            do_stage1(board, &next);
        }
    }

    return EXIT_SUCCESS;            // exit program with the success code

    /* Algorithms are fun :) */
}