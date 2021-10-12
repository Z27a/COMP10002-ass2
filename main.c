#include "ass2.h"

int
main(int argc, char *argv[]) {
    //TODO: Add function separator lines after i've finished
    // What if only input is A or P

    // YOUR IMPLEMENTATION OF STAGES 0-2
    board_t board;
    nxt_act_t next;
    next.num_turns = 1;
    next.prev_turn = '\0';

    do_stage0(board, &next);

    if (next.action == 1) {
        do_stage1(board, &next);
    } else if (next.action == 2) {
        for (int i = 0; i < COMP_ACTIONS; i++) {
            do_stage1(board, &next);
        }
    }

    return EXIT_SUCCESS;            // exit program with the success code
}

/* THE END -------------------------------------------------------------------*/