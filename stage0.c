#include "ass2.h"

/* Stage 0 */
void do_stage0(board_t board, nxt_act_t *nxt_act) {

    init_board(board);

    /* Print initial board config and info */
    printf("BOARD SIZE: %dx%d\n", BOARD_SIZE, BOARD_SIZE);
    printf("#BLACK PIECES: %d\n",
           NUM_PIECES); //TODO: #define these strings. Also use preincrement for off by one situations?
    printf("#WHITE PIECES: %d\n", NUM_PIECES);
    prt_board(board);

    /* Read input and print out changes to board. Print errors if input is not
     * valid */
    prt_from_input(board, nxt_act);
}


/* Loads the initial configuration into the board */
void init_board(board_t board) {
    /* Assumes that the top left cell is always white and white pieces go
     * on the top of tbe board */

    /* Loop through rows and add values */
    int top, mid, btm;

    /* Top rows with white pieces */
    for (top = 0; top < ROWS_WITH_PIECES; top++) {
        fill_pieces(((top + 1) % 2 == 0), top, board, CELL_WPIECE);
    }
    /* Middle rows with no pieces */
    for (mid = top; mid < BOARD_SIZE - ROWS_WITH_PIECES; mid++) {
        for (int cell = 0; cell < BOARD_SIZE; cell++) {
            board[mid][cell] = CELL_EMPTY;
        }
    }
    /* Bottom rows with black pieces */
    for (btm = mid; btm < BOARD_SIZE; btm++) {
        fill_pieces(((btm + 1) % 2 == 0), btm, board, CELL_BPIECE);
    }
}


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


/* Prints the board with formatting */
void prt_board(board_t board) {
    /* Column letters */
    pad();
    for (int col = 0; col < BOARD_SIZE; col++) {
        printf("  %c", 'A' + col);
        if (col + 1 < BOARD_SIZE) {
            printf(" ");
        } else {
            newline();
        }
    }

    /* initial row separator */
    pad();
    printf("%s", ROW_SEP);
    newline();

    /* Rest of the rows */
    for (int row = 0; row < BOARD_SIZE; row++) {
        /* Row number */
        printf(" %d ", row + 1);

        /* Board contents */
        for (int col = 0; col < BOARD_SIZE; col++) {
            printf("%s %c ", COL_SEP, board[row][col]);
            if (col + 1 == BOARD_SIZE) {
                printf("%s", COL_SEP);
                newline();
            }
        }

        /* Row separator */
        pad();
        printf("%s", ROW_SEP);
        newline();
    }
}


/* Read moves and prints out the data of the board after each move. Prints
 * error messages if a move is invalid */
void prt_from_input(board_t board, nxt_act_t *nxt_act) {
    char c1, c2, c3, c4;
    move_t move;
    int first_turn = TRUE;
    int cost;
    int win = 0;

    while (scanf("%c%c-%c%c\n", &c1, &c2, &c3, &c4) == 4) {
        /* Determine the initial value of prev_turn (inverse of first player) */
        if (first_turn) {
            if (same_colour(board[c1 - 'A'][c2 - '0'], CELL_WPIECE)) {
                nxt_act->prev_turn = CELL_BPIECE;
            } else {
                nxt_act->prev_turn = CELL_WPIECE;
            }
            first_turn = FALSE;
        }

        /* Assign values to move */
        move.from.col = c1 - 'A'; //TODO: update the As and 0s with #defines
        move.from.row = c2 - '0' - 1;
        move.to.col = c3 - 'A';
        move.to.row = c4 - '0' - 1;

        /* Check if move is valid */
//        win = check_win(board, prev_turn);
        if (win == FALSE) {
            move_valid(board, &move, nxt_act->prev_turn, FALSE);
        }
        move_valid(board, &move, nxt_act->prev_turn, FALSE);
        /* Update board */
        update_board(board, &move);
        /* Check for a winning state */

        /* Get cost */
        if (win == 1) {
            /* White win */
            cost = INT_MIN;
        } else if (win == 2) {
            cost = INT_MAX;
        } else {
            cost = get_cost(board);
        }

        prt_inb(board, nxt_act, &move, cost, FALSE);

        if (win == 1) {
            /* White win */
            printf("WHITE WIN\n");
        } else if (win == 2) {
            printf("BLACK WIN\n");
        }
        (nxt_act->num_turns)++;
    }

    if (c1 == 'A') {
        nxt_act->action = 1;
    } else if (c1 == 'P') {
        nxt_act->action = 2;
    } else {
        nxt_act->action = 0;
    }
    // TODO: Need to check if a player wins every time?
}

void
prt_inb(board_t board, nxt_act_t *nxt_act, move_t *move, int cost, int is_s1) {
    /* Print board */
    printf("=====================================\n");
    if (is_s1) printf("*** ");
    /* Update prev_turn */
    if (nxt_act->prev_turn == CELL_WPIECE) {
        printf("BLACK ACTION ");
    } else {
        printf("WHITE ACTION ");
    }
    nxt_act->prev_turn = switch_colour(nxt_act->prev_turn);

    printf("#%d: ", nxt_act->num_turns);
    prt_move(move);

    printf("BOARD COST: %d\n", cost);
    prt_board(board);
}


/* Checks if a move is valid */
int move_valid(board_t board, move_t *move, char prev_turn, int not_exit) {
    char from_piece = board[move->from.row][move->from.col];

    /* 1 Source cell is outside of the board. */
    if (outside_board(&(move->from))) {
        //TODO: need to #define these errors?
        if (not_exit) return 0;
        printf("ERROR: Source cell is outside of the board.\n");
        exit(EXIT_FAILURE);
    }

    /* 2 Target cell is outside of the board. */
    if (outside_board(&(move->to))) {
        if (not_exit) return 0;
        printf("ERROR: Target cell is outside of the board.\n");
        exit(EXIT_FAILURE);
    }

    /* 3 Source cell is empty. */
    if (cell_empty(board, &(move->from))) {
        if (not_exit) return 0;
        printf("ERROR: Source cell is empty.\n");
        exit(EXIT_FAILURE);
    }

    /* 4 Target cell is not empty. */
    if (!cell_empty(board, &(move->to))) {
        if (not_exit) return 0;
        //printf("%d %d %d %d", move->from.row, move->from.col, move->to.row, move->to.col);
        printf("ERROR: Target cell is not empty.\n");
        exit(EXIT_FAILURE);
    }

    /* 5 Source cell holds opponent’s piece/tower. */
    if (same_colour(prev_turn, from_piece)) {
        if (not_exit) return 0;
        printf("ERROR: Source cell holds opponent's piece/tower.\n");
        exit(EXIT_FAILURE);
    }

    /* 6 Illegal action. */
    if (!legal_action(board, move, from_piece)) {
        if (not_exit) return 0;
        printf("ERROR: Illegal action.\n");
        exit(EXIT_FAILURE);
    }
    return 1;
}

/* Checks if a coordinate lies outside the board */
int outside_board(coord_t *coord) {
    if (coord->col >= BOARD_SIZE || coord->col < 0 ||
        coord->row >= BOARD_SIZE || coord->row < 0) {
        return TRUE;
    }
    return FALSE;
}

/* Checks if a cell is empty */
int cell_empty(board_t board, coord_t *coord) {
    if (board[coord->row][coord->col] == CELL_EMPTY) {
        //printf("here, %d %d, board: '%c'\n", coord->row, coord->col, board[coord->row][coord->col]);
        return TRUE;
    }
    return FALSE;
}

int same_colour(char c1, char c2) {
    if (lower(c1) == lower(c2)) {
        return 1;
    }
    return 0;
}

/* Return the lowercase version of letters */
char lower(char c) {
    if (is_upper(c)) {
        return (c + 32);
    }
    return c;
}

/* Return the uppercase version of letters */
char upper(char c) {
    if (!is_upper(c)) {
        return (c - 32);
    }
    return c;
}

/* Checks if a char is upper case */
int is_upper(char c) {
    if ('A' <= c && c <= 'Z') {
        return 1;
    }
    return 0;
}

/* Checks if an action is legal */
int legal_action(board_t board, move_t *move, char from_piece) {
    char from_colour = lower(from_piece);
    int dir;
    if (from_colour == CELL_WPIECE) {
        dir = +1;
    } else {
        dir = -1;
    }
    coord_t dist = get_dist(move);
    char cap_piece = lower(
            board[move->from.row + dist.row / 2][move->from.col +
                                                 dist.col / 2]);
    if (dist.row == dir && (abs(dist.col) == 1)) {
        /* Move is one cell diagonally */
        return 1;
    }
    if (dist.row == dir * 2 && (abs(dist.col) == 2)) {
        /* Move is two cells diagonally */
        if (cap_piece != CELL_EMPTY && !same_colour(from_colour, cap_piece)) {
            /* Captured cell is not empty and captured piece is the other
             * player's */
            return 1;
        }
    }
    if (is_upper(from_piece)) {
        //printf("yea\n");
        /* Tower */
        if (dist.row == -dir && (abs(dist.col) == 1)) {
            /* Move is one cell diagonally */
            return 1;
        }
        if (dist.row == -dir * 2 && (abs(dist.col) == 2)) {
            /* Move is two cells diagonally */
            if (cap_piece != CELL_EMPTY &&
                !same_colour(from_colour, cap_piece)) {
                /* Captured cell is not empty and captured piece is the other
                 * player's */
                return 1;
            }
        }
    }
    return 0;
}

/* Gets the distance between two cells */
coord_t get_dist(move_t *move) {
    coord_t dist;
    dist.row = move->to.row - move->from.row;
    dist.col = move->to.col - move->from.col;
    return dist;
}

/* Updates the board after a move has taken place */
void update_board(board_t board, move_t *move) {
    coord_t dist = get_dist(move);
    char moved_piece = board[move->from.row][move->from.col];

    /* 'To' cell takes value of 'from' cell */
    if ((move->to.row == BOARD_SIZE - 1 || move->to.row == 0) &&
        !is_upper(moved_piece)) {
        /* Normal piece becomes a tower piece */
        board[move->to.row][move->to.col] = upper(
                board[move->from.row][move->from.col]);
    } else {
        /* Still a normal piece */
        board[move->to.row][move->to.col] = board[move->from.row][move->from.col];
    }

    /* Set 'from' cell to empty */
    board[move->from.row][move->from.col] = CELL_EMPTY;
    //printf("at update board: %d %d %d %d\n", move->from.row, move->from.col, move->to.row, move->to.col);
    //printf("dist?? %d %d\n", dist.row, dist.col);
    if (abs(dist.row) > 1) {
        /* Make captured cell empty */
        board[move->from.row + dist.row / 2][move->from.col +
                                             dist.col / 2] = CELL_EMPTY;
    }
}

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

int check_win(board_t board, char cur_turn) {
    move_ary psbl_mvs;
    int piece_found = FALSE;
    int valid_mv_found = FALSE;
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            if (lower(board[row][col]) == switch_colour(cur_turn)) {
                piece_found = TRUE;
            }
            if (lower(board[row][col]) == cur_turn) {
                /* Get possible moves */
                psbl_mvs = get_moves(row, col);
                for (int i = 0; i < MAX_MOVES; i++) {
                    if (move_valid(board, &psbl_mvs.moves[i],
                                   switch_colour(cur_turn), TRUE)) {
                        valid_mv_found = TRUE;
                    }
                }
            }
        }
    }
    if (piece_found == FALSE || (piece_found = TRUE && valid_mv_found == FALSE)) {
        /* Winning condition */
        if (cur_turn == CELL_WPIECE) {
            return 1;
        } else {
            return 2;
        }
    }
    return 0;
}


/* Adds padding to output */
            void pad() {
                printf("   ");
            }

/* Adds newline to output */
            void newline() {
                printf("\n");
            }