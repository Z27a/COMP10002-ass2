#include "ass2.h"

/* Stage 0 */
void do_stage0(board_t board, nxt_act_t *nxt_act) {

    init_board(board);

    prt_starting_info();

    prt_board(board);

    read_and_prt(board, nxt_act);
}

/*----------------------------------------------------------------------------*/
/* Loads the initial configuration into the board */
void init_board(board_t board) {
    /* Loop through rows to add values */
    int top, mid, btm;

    /* Top rows with white pieces */
    for (top = 0; top < ROWS_WITH_PIECES; top++) {
        fill_pieces(((top + IND2ROW) % 2 == 0), top, board, CELL_WPIECE);
    }
    /* Middle rows with no pieces */
    for (mid = top; mid < BOARD_SIZE - ROWS_WITH_PIECES; mid++) {
        for (int cell = 0; cell < BOARD_SIZE; cell++) {
            board[mid][cell] = CELL_EMPTY;
        }
    }
    /* Bottom rows with black pieces */
    for (btm = mid; btm < BOARD_SIZE; btm++) {
        fill_pieces(((btm + IND2ROW) % 2 == 0), btm, board, CELL_BPIECE);
    }
}

/*----------------------------------------------------------------------------*/
/* Fills in a row that contains pieces */
void fill_pieces(int row_even, int row, board_t board, char piece) {
    int was_piece;

    if (row_even) {
        /* Row starts with a piece */
        was_piece = FALSE;
    } else {
        /* Row doesn't start with a piece */
        was_piece = TRUE;
    }

    for (int col = 0; col < BOARD_SIZE; col++) {
        if (was_piece) {
            /* Current cell is empty */
            board[row][col] = CELL_EMPTY;
            was_piece = FALSE;
        } else {
            /* Current cell is a piece */
            board[row][col] = piece;
            was_piece = TRUE;
        }
    }
}

/*----------------------------------------------------------------------------*/
/* Print initial game configuration info */
void prt_starting_info() {
    printf("BOARD SIZE: %dx%d\n", BOARD_SIZE, BOARD_SIZE);
    printf("#BLACK PIECES: %d\n", NUM_PIECES);
    printf("#WHITE PIECES: %d\n", NUM_PIECES);
    /* Well, here's the #defined version:
     *
     - #define BDSZMSG "BOARD SIZE:"
     - #define BPCEMSG "#BLACK PIECES:"
     - #define WPCEMSG "#WHITE PIECES:"
     - printf("%s%s%dx%d%s", BDSZMSG, SPACE, BOARD_SIZE, BOARD_SIZE, NEWLINE);
     - printf("%s%s%d%s", BPCEMSG, SPACE, NUM_PIECES, NEWLINE);
     - printf("%s%s%d%s", WPCEMSG, SPACE, NUM_PIECES, NEWLINE);
     *
     * but I think it's a lot less readable, and I hope I can convince you of
     * that as well, so I won't be #defining some stuff in this assignment */
}

/*----------------------------------------------------------------------------*/
/* Prints the board with formatting */
void prt_board(board_t board) {
    /* Column letters */
    printf("%s", PADDING);
    for (int col = 0; col < BOARD_SIZE; col++) {
        printf("%s%s%c", SPACE, SPACE, col + IND2COL);
        if (col + 1 < BOARD_SIZE) {
            printf("%s", SPACE);
        } else {
            printf("%s", NEWLINE);
        }
    }

    /* First row separator */
    printf("%s%s%s", PADDING, ROW_SEP, NEWLINE);

    /* Rest of the rows */
    for (int row = 0; row < BOARD_SIZE; row++) {
        /* Row number */
        printf("%s%d ", SPACE, row + IND2ROW);

        /* Board contents */
        for (int col = 0; col < BOARD_SIZE; col++) {
            printf("%s%s%c ", COL_SEP, SPACE, board[row][col]);
            if (col + 1 == BOARD_SIZE) {
                printf("%s", COL_SEP);
                printf("%s", NEWLINE);
            }
        }

        /* Row separator */
        printf("%s%s%s", PADDING, ROW_SEP, NEWLINE);
    }
}

/*----------------------------------------------------------------------------*/
/* Read moves and prints out the data of the board after each move. Prints
 * error messages if a move is invalid */
void read_and_prt(board_t board, nxt_act_t *nxt_act) {
    char c1, c2, c3, c4;
    move_t move;
    int first_turn = TRUE;

    while (scanf("%c%c-%c%c\n", &c1, &c2, &c3, &c4) == 4) {
        /* Assign values to move struct */
        move.from.col = c1 - IND2COL;
        move.from.row = atoi(&c2) - IND2ROW;
        move.to.col = c3 - IND2COL;
        move.to.row = atoi(&c4) - IND2ROW;

        /* Determine the initial value of prev_turn (inverse of first player) */
        if (first_turn) {
            if (same_colour(board[move.from.row][move.from.col], CELL_WPIECE)) {
                nxt_act->prev_turn = CELL_BPIECE;
            } else {
                nxt_act->prev_turn = CELL_WPIECE;
            }
            first_turn = FALSE;
        }

        move_valid(board, &move, nxt_act->prev_turn, FALSE);

        update_board(board, &move);

        prt_bd_inf(board, nxt_act, &move, FALSE);

        /* The turn has ended, increment num_turns */
        nxt_act->num_turns++;
    }
    /* No more moves are entered, determine the next instructions */
    if (c1 == STAGE1CMD) {
        nxt_act->is_stage1 = TRUE;
    } else if (c1 != STAGE2CMD) {
        exit(EXIT_SUCCESS);
    }
}

/*----------------------------------------------------------------------------*/
/* Print the board and current game info */
void prt_bd_inf(board_t board, nxt_act_t *nxt_act, move_t *move,
                int is_stage1) {
    /* Print all the required content */
    printf("%s%s", BOARD_SEP, NEWLINE);

    if (is_stage1) printf("%s%s", COMP_ACT_SIG, SPACE);

    if (nxt_act->prev_turn == CELL_WPIECE) {
        printf("BLACK ACTION%s", SPACE);
    } else {
        printf("WHITE ACTION%s", SPACE);
    }

    printf("#%d:%s", nxt_act->num_turns, SPACE);

    prt_move(move);

    printf("BOARD COST: %d\n", get_cost(board));

    prt_board(board);

    /* Check for a winner. Print winner and exit if there is one */
    if (has_won(board, nxt_act->prev_turn)) {
        if (nxt_act->prev_turn == CELL_WPIECE) {
            printf("BLACK WIN!\n");
        } else {
            printf("WHITE WIN!\n");
        }
        exit(EXIT_SUCCESS);
    }

    /* Update prev_turn now that a turn has ended */
    nxt_act->prev_turn = switch_colour(nxt_act->prev_turn);
}

/*----------------------------------------------------------------------------*/
/* Prints all values of a move struct */
void prt_move(move_t *move) {
    char m1, m2, m3, m4;
    m1 = move->from.col + IND2COL;
    m2 = move->from.row + IND2ROW;
    m3 = move->to.col + IND2COL;
    m4 = move->to.row + IND2ROW;
    printf("%c%d-%c%d\n", m1, m2, m3, m4);
}

/*----------------------------------------------------------------------------*/
/* Checks if a move is valid.Can either return a bool or exit the program */
int move_valid(board_t board, move_t *move, char prev_turn, int not_exit) {
    char from_piece = board[move->from.row][move->from.col];

    /* 1 Source cell is outside of the board. */
    if (outside_board(&(move->from))) {
        if (not_exit) return FALSE;
        printf("%s%s", ERR1, NEWLINE);
        exit(EXIT_FAILURE);
    }
    /* 2 Target cell is outside of the board. */
    if (outside_board(&(move->to))) {
        if (not_exit) return FALSE;
        printf("%s%s", ERR2, NEWLINE);
        exit(EXIT_FAILURE);
    }
    /* 3 Source cell is empty. */
    if (cell_empty(board, &(move->from))) {
        if (not_exit) return FALSE;
        printf("%s%s", ERR3, NEWLINE);
        exit(EXIT_FAILURE);
    }
    /* 4 Target cell is not empty. */
    if (!cell_empty(board, &(move->to))) {
        if (not_exit) return FALSE;
        printf("%s%s", ERR4, NEWLINE);
        exit(EXIT_FAILURE);
    }
    /* 5 Source cell holds opponent’s piece/tower. */
    if (same_colour(prev_turn, from_piece)) {
        if (not_exit) return FALSE;
        printf("%s%s", ERR5, NEWLINE);
        exit(EXIT_FAILURE);
    }
    /* 6 Illegal action. */
    if (!legal_action(board, move, from_piece)) {
        if (not_exit) return FALSE;
        printf("%s%s", ERR6, NEWLINE);
        exit(EXIT_FAILURE);
    }
    /* No errors */
    return TRUE;
}

/*----------------------------------------------------------------------------*/
/* Checks if a coordinate lies outside the board */
int outside_board(coord_t *coord) {
    if (coord->col >= BOARD_SIZE || coord->col < 0 ||
        coord->row >= BOARD_SIZE || coord->row < 0) {
        return TRUE;
    }
    return FALSE;
}

/*----------------------------------------------------------------------------*/
/* Checks if a cell is empty */
int cell_empty(board_t board, coord_t *coord) {
    if (board[coord->row][coord->col] == CELL_EMPTY) {
        return TRUE;
    }
    return FALSE;
}

/*----------------------------------------------------------------------------*/
/* Checks if two pieces are the same colour */
int same_colour(char c1, char c2) {
    if (lower(c1) == lower(c2)) {
        return TRUE;
    }
    return FALSE;
}

/*----------------------------------------------------------------------------*/
/* Checks if a char is upper case */
int is_upper(char c) {
    if ('A' <= c && c <= 'Z') {
        return TRUE;
    }
    return FALSE;
}

/*----------------------------------------------------------------------------*/
/* Return the lowercase version of letters */
char lower(char c) {
    if (is_upper(c)) {
        return (c + LC_UC_DIFF);
    }
    return c;
}

/*----------------------------------------------------------------------------*/
/* Return the uppercase version of letters */
char upper(char c) {
    if (!is_upper(c)) {
        return (c - LC_UC_DIFF);
    }
    return c;
}

/*----------------------------------------------------------------------------*/
/* Checks if a move is legal */
int legal_action(board_t board, move_t *move, char from_piece) {
    char from_colour = lower(from_piece);
    int dir;

    /* Determine the direction of allowed movement (non-tower) */
    if (from_colour == CELL_WPIECE) {
        dir = +1; // South
    } else {
        dir = -1; // North

    }
    /* Determine how far the move is */
    coord_t dist = get_dist(move);

    /* Loop through valid moves and test if the provided move is one of them */
    for (int i = 0; i < MAX_DIR; i++) {
        if ((dist.row == dir) && (abs(dist.col) == 1)) {
            /* Move is one cell diagonally */
            return 1;
        }

        if ((dist.row == dir * MAX_DIST) && (abs(dist.col) == MAX_DIST)) {
            /* Move is two cells diagonally */
            /* Get the piece that is captured (1/2 of the distance of the
             * move) */
            char cap_piece = board[move->from.row + dist.row / 2][
                                   move->from.col + dist.col / 2];

            if (cap_piece != CELL_EMPTY &&
                !same_colour(from_colour, cap_piece)) {
                /* Captured cell is not empty and captured piece is the other
                 * player's */
                return 1;
            }
        }

        if (!is_upper(from_piece)) {
            /* Not a tower, stop testing */
            break;
        } else {
            /* Run the loop again to check for the moves in the opposite
             * direction that towers can make */
            dir = -dir;
        }
    }
    /* Move is not valid */
    return 0;
}

/*----------------------------------------------------------------------------*/
/* Returns the vector distance of a move */
coord_t get_dist(move_t *move) {
    coord_t dist;
    dist.row = move->to.row - move->from.row;
    dist.col = move->to.col - move->from.col;
    return dist;
}

/*----------------------------------------------------------------------------*/
/* Updates the board with the new move */
void update_board(board_t board, move_t *move) {
    coord_t dist = get_dist(move);

    /* 'To' cell takes value of 'from' cell */
    if ((move->to.row == BOARD_SIZE - 1 || move->to.row == 0) &&
        !is_upper(board[move->from.row][move->from.col])) {
        /* Piece is at the top or bottom row of the board and isn't a tower.
         * Normal piece becomes a tower piece */
        board[move->to.row][move->to.col] = upper(
                board[move->from.row][move->from.col]);
    } else {
        /* Still a normal piece */
        board[move->to.row][move->to.col] =
                board[move->from.row][move->from.col];
    }

    /* Set 'from' cell to empty */
    board[move->from.row][move->from.col] = CELL_EMPTY;

    if (abs(dist.row) > 1) {
        /* A capture has happened, make captured cell empty */
        board[move->from.row + dist.row / 2][move->from.col +
                                             dist.col / 2] = CELL_EMPTY;
    }
}

/*----------------------------------------------------------------------------*/
/* Calculate cost by Looping through board and counting pieces */
int get_cost(board_t board) {
    int cost = 0;
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            if (board[row][col] == CELL_EMPTY) {
                continue;
            } else if (board[row][col] == CELL_WPIECE) {
                cost -= COST_PIECE;
            } else if (board[row][col] == CELL_BPIECE) {
                cost += COST_PIECE;
            } else if (board[row][col] == CELL_WTOWER) {
                cost -= COST_TOWER;
            } else {
                cost += COST_TOWER;
            }
        }
    }
    return cost;
}

/*----------------------------------------------------------------------------*/
/* Determine if a player has won */
int has_won(board_t board, char nxt_turn) {
    move_ary psbl_mvs;
    int piece_found = FALSE;
    int valid_mv_found = FALSE;

    /* Loop through board and find pieces that match the colour of the next
     * turn. Then see if those pieces can be moved */
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            if (same_colour(board[row][col], nxt_turn)) {
                piece_found = TRUE;

                /* Find possible moves */
                psbl_mvs = get_moves(row, col);

                /* Loop through possible moves and test if they are valid */
                for (int i = 0; i < MAX_MOVES; i++) {
                    if (move_valid(board, &psbl_mvs.moves[i],
                                   switch_colour(nxt_turn), TRUE)) {
                        valid_mv_found = TRUE;
                    }
                }
            }
        }
    }
    if (piece_found == FALSE ||
        (piece_found = TRUE && valid_mv_found == FALSE)) {
        /* Winning condition - opponent has no pieces left or their pieces
         * can't move */
        return TRUE;
    }
    return FALSE;
}

/*----------------------------------------------------------------------------*/
/* Return all possible (not necessarily valid) moves of a given coordinate */
move_ary get_moves(int row, int col) {
    move_ary new;
    /* "From" coordinate doesn't change, so copy the original */
    for (int i = 0; i < MAX_MOVES; i++) {
        new.moves[i].from.row = row;
        new.moves[i].from.col = col;
    }
    /* Add moves, starting from length BASE_MOVE to MAX_DIST from NE to NW
     * in a clockwise direction */
    for (int i = 0; i < MAX_DIST; i++) {
        /* NE */
        new.moves[0 + i].to.row = row - (i + BASE_MOVE);
        new.moves[0 + i].to.col = col + (i + BASE_MOVE);
    }
    for (int i = 0; i < MAX_DIST; i++) {
        /* SE */
        new.moves[2 + i].to.row = row + (i + BASE_MOVE);
        new.moves[2 + i].to.col = col + (i + BASE_MOVE);
    }
    for (int i = 0; i < MAX_DIST; i++) {
        /* SW */
        new.moves[4 + i].to.row = row + (i + BASE_MOVE);
        new.moves[4 + i].to.col = col - (i + BASE_MOVE);
    }
    for (int i = 0; i < MAX_DIST; i++) {
        /* NW */
        new.moves[6 + i].to.row = row - (i + BASE_MOVE);
        new.moves[6 + i].to.col = col - (i + BASE_MOVE);
    }
    return new;
}
