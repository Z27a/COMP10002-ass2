/* Program to print and play checker games.

  Skeleton program written by Artem Polyvyanyy, artem.polyvyanyy@unimelb.edu.au,
  September 2021, with the intention that it be modified by students
  to add functionality, as required by the assignment specification.

  Student Authorship Declaration:

  (1) I certify that except for the code provided in the initial skeleton file,
  the program contained in this submission is completely my own individual
  work, except where explicitly noted by further comments that provide details
  otherwise. I understand that work that has been developed by another student,
  or by me in collaboration with other students, or by non-students as a result
  of request, solicitation, or payment, may not be submitted for assessment in
  this subject. I understand that submitting for assessment work developed by
  or in collaboration with other students or non-students constitutes Academic
  Misconduct, and may be penalized by mark deductions, or by other penalties
  determined via the University of Melbourne Academic Honesty Policy, as
  described at https://academicintegrity.unimelb.edu.au.

  (2) I also certify that I have not provided a copy of this work in either
  softcopy or hardcopy or any other form to any other student, and nor will I
  do so until after the marks are released. I understand that providing my work
  to other students, regardless of my intention or any undertakings made to me
  by that other student, is also Academic Misconduct.

  (3) I further understand that providing a copy of the assignment specification
  to any form of code authoring or assignment tutoring service, or drawing the
  attention of others to such services and code that may have been made
  available via such a service, may be regarded as Student General Misconduct
  (interfering with the teaching activities of the University and/or inciting
  others to commit Academic Misconduct). I understand that an allegation of
  Student General Misconduct may arise regardless of whether or not I personally
  make use of such solutions or sought benefit from such actions.

  Signed by: Zhirong Lin 1264039
  Dated:     04/10/2021

*/

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <assert.h>

/* some #define's from my sample solution ------------------------------------*/
#define BOARD_SIZE          8       // board size
#define ROWS_WITH_PIECES    3       // number of initial rows with pieces
#define CELL_EMPTY          '.'     // empty cell character
#define CELL_BPIECE         'b'     // black piece character
#define CELL_WPIECE         'w'     // white piece character
#define CELL_BTOWER         'B'     // black tower character
#define CELL_WTOWER         'W'     // white tower character
#define COST_PIECE          1       // one piece cost
#define COST_TOWER          3       // one tower cost
#define TREE_DEPTH          3       // minimax tree depth
#define COMP_ACTIONS        100      // number of computed actions

/* one type definition from my sample solution -------------------------------*/
typedef unsigned char board_t[BOARD_SIZE][BOARD_SIZE];  // board type

/* No longer skeleton code ###################################################*/
#define TRUE 1
#define FALSE 0
#define NUM_PIECES (int) (ROWS_WITH_PIECES*BOARD_SIZE*0.5)
#define ROW_SEP "+---+---+---+---+---+---+---+---+"
#define COL_SEP "|"
#define SPACE " "
#define PADDING "   "
#define NEWLINE "\n"
#define BOARD_SEP "====================================="
#define COMP_ACT_SIG "***" // Signifier for a computed action
#define MAX_MOVES 8
#define MAX_DIR 2
#define MAX_DIST 2
#define BASE_MOVE 1
#define IND2ROW 1 // Index to row number
#define IND2COL 'A' // Index to alphabetical char
#define STAGE1CMD 'A'
#define STAGE2CMD 'P'
#define ERR1 "ERROR: Source cell is outside of the board."
#define ERR2 "ERROR: Target cell is outside of the board."
#define ERR3 "ERROR: Source cell is empty."
#define ERR4 "ERROR: Target cell is not empty."
#define ERR5 "ERROR: Source cell holds opponent's piece/tower."
#define ERR6 "ERROR: Illegal action."
#define LC_UC_DIFF 32 // Int difference between lower case and upper case chars
#define INITIAL_TURNS 1
#define NULL_MOVE '\0'
#define INITIAL_DEPTH 1
#define ROOT_MOVE (-1)


typedef struct {
    int row;
    int col;
} coord_t;

typedef struct {
    coord_t from;
    coord_t to;
} move_t;

typedef struct {
    move_t moves[MAX_MOVES];
} move_ary; // Array of moves

typedef struct {
    int is_stage1;
    char prev_turn;
    int num_turns;
} nxt_act_t; // Used for passing info between stage functions and main

/* These list structs and tree structs are modelled after Alistair Moffat's code
 * from https://people.eng.unimelb.edu.au/ammoffat/ppsaa/c/ */

typedef struct lst_node lst_node_t;
typedef struct state state_t; // A state of the game. It's a tree node

struct lst_node {
    state_t *data;
    lst_node_t *next;
};

typedef struct {
    lst_node_t *head;
    lst_node_t *foot;
} lst_t;

struct state {
    board_t board;
    int cost;
    move_t move;
    char cur_turn;
    lst_t *child_hdl;
};

typedef struct {
    int cost;
    move_t move;
} cam_t; // Stands for cost and move


/* Stage 0 */
void do_stage0(board_t board, nxt_act_t *nxt_act);

void init_board(board_t board);

void prt_starting_info();

void fill_pieces(int row_even, int row, board_t board, char piece);

void prt_board(board_t board);

void read_and_prt(board_t board, nxt_act_t *nxt_act);

void prt_move(move_t *move);

int move_valid(board_t board, move_t *move, char prev_turn, int not_exit);

void update_board(board_t board, move_t *move);

void prt_bd_inf(board_t board, nxt_act_t *nxt_act, move_t *move, int is_stage1);

int outside_board(coord_t *coord);

int cell_empty(board_t board, coord_t *coord);

int same_colour(char c1, char c2);

int is_upper(char c);

char lower(char c);

char upper(char c);

int legal_action(board_t board, move_t *move, char from_piece);

coord_t get_dist(move_t *move);

int get_cost(board_t board);

int has_won(board_t board, char nxt_turn);

move_ary get_moves(int row, int col);

char switch_colour(char orig);

/* Stage 1 */
void do_stage1(board_t board, nxt_act_t *nxt_act);

void build_tree(state_t *parent, int depth);

state_t *new_child(board_t prev_board, move_t *move, char prev_turn);

state_t *init_root(board_t board, char cur_turn);

lst_t *new_handle();

void insert_child(lst_t *handle, state_t *data);

cam_t run_minimax(state_t *state, char player);

void free_tree(state_t *parent);

void board_cpy(board_t orig, board_t new);

void move_cpy(move_t *orig, move_t *new);

/*----------------------------------------------------------------------------*/
int
main(int argc, char *argv[]) {
    // YOUR IMPLEMENTATION OF STAGES 0-2
    board_t board;
    nxt_act_t next;
    /* Set starting values */
    next.num_turns = INITIAL_TURNS;
    next.prev_turn = NULL_MOVE;
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

/*----------------------------------------------------------------------------*/
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

/*----------------------------------------------------------------------------*/
/* Changes the colour from black to white or white to black */
char switch_colour(char orig) {
    char new;
    if (orig == CELL_WPIECE) {
        new = CELL_BPIECE;
    } else {
        new = CELL_WPIECE;
    }
    return new;
}


/*----------------------------------------------------------------------------*/
/* The list functions and tree functions used here are modelled after Alistair
 * Moffat's code from https://people.eng.unimelb.edu.au/ammoffat/ppsaa/c/ */

/* Stage 1 */
void do_stage1(board_t board, nxt_act_t *nxt_act) {
    if (nxt_act->prev_turn == NULL_MOVE) {
        /* No moves have been read. Let black start first (i.e. last turn was
         * white)*/
        nxt_act->prev_turn = CELL_WPIECE;
    }

    state_t *root = init_root(board, switch_colour(nxt_act->prev_turn));

    build_tree(root, INITIAL_DEPTH);

    /* Calculate the next most optimal move */
    cam_t cam = run_minimax(root, root->cur_turn);

    free_tree(root);

    update_board(board, &cam.move);

    prt_bd_inf(board, nxt_act, &cam.move, TRUE);

    nxt_act->num_turns++;
}

/*----------------------------------------------------------------------------*/
/* Constructs the minimax tree */
void build_tree(state_t *parent, int depth) {
    if (depth > TREE_DEPTH) {
        /* Base case - max depth reached */
        return;
    } else {
        state_t *new;
        move_ary psbl_mvs;
        int piece_found = FALSE;
        int valid_move_found = FALSE;

        /* Loop through board and find pieces that match the colour of the
         * current turn. */
        for (int row = 0; row < BOARD_SIZE; row++) {
            for (int col = 0; col < BOARD_SIZE; col++) {
                if (same_colour(parent->board[row][col], parent->cur_turn)) {
                    piece_found = TRUE;

                    /* Find possible moves */
                    psbl_mvs = get_moves(row, col);

                    /* Loop through possible moves */
                    for (int i = 0; i < MAX_MOVES; i++) {
                        if (move_valid(parent->board, &psbl_mvs.moves[i],
                                       switch_colour(parent->cur_turn), TRUE)) {
                            valid_move_found = TRUE;

                            /* Create a child node with the current move */
                            new = new_child(parent->board, &psbl_mvs.moves[i],
                                            parent->cur_turn);

                            insert_child(parent->child_hdl, new);

                            /* Run build tree on the new child */
                            build_tree(new, depth + 1);
                        }
                    }
                }
            }
        }
        if (piece_found == FALSE ||
            (piece_found = TRUE && valid_move_found == FALSE)) {
            /* Winning condition - opponent has no pieces left or their pieces
             * can't move. Set the parent's cost to INT_MAX or INT_MIN */
            if (parent->cur_turn == CELL_WPIECE) {
                parent->cost = INT_MAX;
            } else {
                parent->cost = INT_MIN;
            }
        }
    }
}

/*----------------------------------------------------------------------------*/
/* Mallocs a new root state and sets the initial data for the root   */
state_t *init_root(board_t board, char cur_turn) {
    state_t *root = (state_t *) malloc(sizeof(*root));
    assert(root != NULL);
    board_cpy(board, root->board);
    root->cost = get_cost(board);
    root->cur_turn = cur_turn;
    root->child_hdl = new_handle();
    root->move.from.row = ROOT_MOVE;
    return root;
}

/*----------------------------------------------------------------------------*/
/* Mallocs a new child state and sets the data of this node based on a given
 * move that got to the child state */
state_t *new_child(board_t prev_board, move_t *move, char prev_turn) {
    state_t *new = (state_t *) malloc(sizeof(*new));
    assert(new != NULL);
    board_cpy(prev_board, new->board);
    update_board(new->board, move);
    new->cost = get_cost(new->board);
    move_cpy(move, &new->move);
    new->cur_turn = switch_colour(prev_turn);
    new->child_hdl = new_handle();
    return new;
}

/*----------------------------------------------------------------------------*/
/* Mallocs a new linked list handle and sets its initial values */
lst_t *new_handle() {
    lst_t *handle;
    handle = (lst_t *) malloc(sizeof(*handle));
    assert(handle != NULL);
    handle->head = handle->foot = NULL;
    return handle;
}

/*----------------------------------------------------------------------------*/
/* Insert a child into a parent's linked list. Also creates a new list node */
void insert_child(lst_t *handle, state_t *data) {
    lst_node_t *new = (lst_node_t *) malloc(sizeof(*new));
    assert(new != NULL);
    new->next = NULL;
    new->data = data;

    if (handle->foot == NULL) {
        /* The list's first insertion */
        handle->head = handle->foot = new;
    } else {
        /* Not the first node in the list */
        handle->foot->next = new;
        handle->foot = new;
    }
}

/*----------------------------------------------------------------------------*/
/* Runs the minimax algorithm. Returns the next best move and back propagated
 * cost of children nodes */
cam_t run_minimax(state_t *state, char player) {
    cam_t cam;
    if (state->child_hdl->head == NULL) {
        /* Base case - the current state has no more children. Return cost and
         * move of current state */
        cam.cost = state->cost;
        cam.move = state->move;
    } else {
        /* Recursive case. */
        /* Run minimax on the first element of the list of child states */
        cam = run_minimax(state->child_hdl->head->data, switch_colour(player));

        if (state->move.from.row != ROOT_MOVE) {
            /* Current state is not the root, return the move of the current
             * state */
            cam.move = state->move;
        } else {
            /* The current state is the root. Return the move of the
             * child state */
            cam.move = state->child_hdl->head->data->move;
        }

        /* Pointer to the first next element in the list */
        lst_node_t *next_elem = state->child_hdl->head->next;
        cam_t temp_cam;

        while (next_elem != NULL) {
            /* Run minimax on all children in the list */
            temp_cam = run_minimax(next_elem->data, switch_colour(player));

            if (player == CELL_WPIECE) {
                /* Minimise cost */
                if (cam.cost > temp_cam.cost) {
                    /* Update cost and move */
                    cam.cost = temp_cam.cost;
                    if (state->move.from.row == ROOT_MOVE) {
                        cam.move = next_elem->data->move;
                    }
                }
            } else {
                /* Maximise cost */
                if (cam.cost < temp_cam.cost) {
                    /* Update cost and move */
                    cam.cost = temp_cam.cost;
                    if (state->move.from.row == ROOT_MOVE) {
                        cam.move = next_elem->data->move;
                    }
                }
            }

            next_elem = next_elem->next;
        }
    }
    return cam;
}

/*----------------------------------------------------------------------------*/
/* Frees all malloced memory in the tree */
void free_tree(state_t *parent) {
    assert(parent != NULL);
    if (parent->child_hdl->head == NULL) {
        /* Base case - no more children to free. */
        free(parent->child_hdl);
        free(parent);
        return;
    } else {
        /* Loop through children and free them. */
        assert(parent->child_hdl != NULL);
        lst_node_t *curr, *prev;
        curr = parent->child_hdl->head;
        while (curr) {
            prev = curr;
            curr = curr->next;
            free_tree(prev->data);
            free(prev);
        }
        /* All children freed, free parent */
        free(parent->child_hdl);
        free(parent);
    }
}

/*----------------------------------------------------------------------------*/
/* Copies the contents of one board onto another by looping through the entire
 * board */
void board_cpy(board_t orig, board_t new) {
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            new[row][col] = orig[row][col];
        }
    }
}

/*----------------------------------------------------------------------------*/
/* Copies the contents of one move to another */
void move_cpy(move_t *orig, move_t *new) {
    new->to = orig->to;
    new->from = orig->from;
}

/* THE END -------------------------------------------------------------------*/