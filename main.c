#include "ass2.h"

int
main(int argc, char *argv[]) {
    //TODO: Add function separator lines after i've finished

    // YOUR IMPLEMENTATION OF STAGES 0-2
    board_t board;
    next_action_t next;

    next = do_stage0(board);

    if (next.action == 1) {
        do_stage1(board, next.turn);
    } else if (next.action == 2) {
        do_stage2(board);
    } else {
        // check for winner?
    }


    return EXIT_SUCCESS;            // exit program with the success code
}

/* THE END -------------------------------------------------------------------*/