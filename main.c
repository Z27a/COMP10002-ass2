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
#define COMP_ACTIONS        10      // number of computed actions

/* one type definition from my sample solution -------------------------------*/
typedef unsigned char board_t[BOARD_SIZE][BOARD_SIZE];  // board type

/* #defines ------------------------------------------------------------------*/
#define TRUE 1
#define FALSE 0
#define NUM_PIECES (ROWS_WITH_PIECES*BOARD_SIZE)

// TODO: make row sep dynamic?
#define ROW_SEP "+---+---+---+---+---+---+---+---+"
#define COL_SEP "|"

/* typedefs ------------------------------------------------------------------*/
typedef struct {
    int row;
    int col;
} coord_t;

typedef struct {
    coord_t from;
    coord_t to;
} move_t;


/* function prototypes -------------------------------------------------------*/
void do_stage0(board_t *board);

void init_board(board_t *board);

void fill_pieces(int row_even, int row, board_t *board, char piece);

void prt_board(board_t *board);

int prt_from_input(board_t *board);

void move_valid(board_t *board, move_t *move, char prev_turn);

int outside_board(coord_t *coord);

int cell_empty(board_t *board, coord_t *coord);

int same_colour(char c1, char c2);

char lower(char c);

int is_upper(char c);

int legal_action(board_t *board, coord_t *from, coord_t *to, char from_piece);

int valid_move(int dir, coord_t *dist, char from_colour, char cap_colour,
               int is_tower);

coord_t diag(coord_t from, int dist, int dir);

int same_coord(coord_t *coord1, coord_t *coord2, coord_t *c_tested);

void pad();

void newline();

int
main(int argc, char *argv[]) {
    //TODO: Add function separator lines after i've finished

    // YOUR IMPLEMENTATION OF STAGES 0-2
    board_t board;

    do_stage0(board);


    return EXIT_SUCCESS;            // exit program with the success code
}


/* Stage 0 */
void do_stage0(board_t *board) {
    int instruction;

    init_board(board);

    /* Print initial board config and info */
    printf("BOARD SIZE: %dx%d\n", BOARD_SIZE, BOARD_SIZE);
    printf("#BLACK PIECES: %d\n", NUM_PIECES);
    printf("#WHITE PIECES: %d\n", NUM_PIECES);
    prt_board(board);

    /* Read input and print out changes to board. Print errors if input is not
     * valid */
    //instruction = prt_from_input(board);
    prt_from_input(board);
    // Need to check if a player wins every time?
}


/* Loads the initial configuration into the board */
void init_board(board_t *board) {
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
            *board[mid][cell] = CELL_EMPTY;
        }
    }
    /* Bottom rows with black pieces */
    for (btm = mid; btm < BOARD_SIZE; btm++) {
        fill_pieces(((btm + 1) % 2 == 0), btm, board, CELL_BPIECE);
    }
}


/* Fills in a row that contains pieces */
void fill_pieces(int row_even, int row, board_t *board, char piece) {
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
            *board[row][col] = CELL_EMPTY;
            was_piece = FALSE;
        } else {
            /* Current cell is a piece */
            *board[row][col] = piece;
            was_piece = TRUE;
        }
    }
}


/* Prints the board with formatting */
void prt_board(board_t *board) {
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
            printf("%s %c ", COL_SEP, *board[row][col]);
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


/* Read moves and prints out the state of the board after each move. Prints
 * error messages if a move is invalid */
int prt_from_input(board_t *board) {
    char c1, c2, c3, c4, turn;
    move_t move;
    int first_turn = TRUE;

    while (scanf("%c%c-%c%c\n", &c1, &c2, &c3, &c4) != EOF) {
        /* Determine the initial value of turn (inverse of first player) */
        if (first_turn) {
            if (same_colour(*board[c1 - 'A'][c2 - '0'], 'w')) {
                turn = 'b';
            } else {
                turn = 'w';
            }
            first_turn = FALSE;
        }

        /* Assign values to move */
        move.from.col = c1 - 'A'; //TODO: update the As and 0s with #defines
        move.from.row = c2 - '0' - 1;
        move.to.col = c3 - 'A';
        move.to.row = c4 - '0' - 1;

        /* Check if move is valid */
        move_valid(board, &move, turn);
        /* Update board */

        /* Print board */

        /* Update turn */
        if (turn == 'w') {
            turn = 'b';
        } else {
            turn = 'w';
        }
    }
    return 0;
}


/* Checks if a move is valid */
void move_valid(board_t *board, move_t *move, char prev_turn) {
    char from_piece = *board[move->from.row][move->from.col];

    /* 1 Source cell is outside of the board. */
    if (outside_board(&(move->from))) {
        //TODO: need to #define these errors?
        printf("1 ERROR: Source cell is outside of the board.\n");
        exit(EXIT_FAILURE);
    }

    /* 2 Target cell is outside of the board. */
    if (outside_board(&(move->to))) {
        printf("2 ERROR: Target cell is outside of the board.\n");
        exit(EXIT_FAILURE);
    }

    /* 3 Source cell is empty. */
    if (cell_empty(board, &(move->from))) {
        printf("3 ERROR: Source cell is empty.\n");
        exit(EXIT_FAILURE);
    }

    /* 4 Target cell is not empty. */
    if (!cell_empty(board, &(move->to))) {
        //printf("%d %d %d %d", move->from.row, move->from.col, move->to.row, move->to.col);
        //printf("4 ERROR: Target cell is not empty.\n");
        exit(EXIT_FAILURE);
    }

    /* 5 Source cell holds opponent’s piece/tower. */
    if (same_colour(prev_turn, from_piece)) {
        printf("5 ERROR: Source cell holds opponent's piece/tower.\n");
        exit(EXIT_FAILURE);
    }

    /* 6 Illegal action. */
    if (!legal_action(board, &(move->from), &(move->to), from_piece)) {
        printf("6 ERROR: Illegal action.\n");
        exit(EXIT_FAILURE);
    }

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
int cell_empty(board_t *board, coord_t *coord) {
    if (*board[coord->row][coord->col] == CELL_EMPTY) {
        //printf("here, %d %d, board: '%c'\n", coord->row, coord->col, *board[coord->row][coord->col]);
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

/* Checks if a char is upper case */
int is_upper(char c) {
    if ('A' <= c && c <= 'Z') {
        return 1;
    }
    return 0;
}

/* Checks if an action is legal */
int legal_action(board_t *board, coord_t *from, coord_t *to, char from_piece) {
    char from_colour = lower(from_piece);

    coord_t dist;
    dist.row = to->row - from->row;
    dist.col = to->col - from->col;

    char cap_colour = lower(*board[from->row + dist.row / 2][from->col +
                                                             dist.col / 2]);

    if (from_colour == 'w') {
        return valid_move(+1, &dist, from_colour, cap_colour,
                          lower(from_piece));
    }
    return valid_move(-1, &dist, from_colour, cap_colour, lower(from_piece));
}


/* Checks if a move is valid. Confusing with legal action existing and all huh
 * dir = +1 for white, -1 for black */
int valid_move(int dir, coord_t *dist, char from_colour, char cap_colour,
               int is_tower) {
    if (dist->row == dir * 1 && (dist->col == 1 || dist->col == -1)) {
        /* Move is one cell diagonally down */
        return 1;
    }
    if (dist->row == dir * 2 && (dist->col == 2 || dist->col == -2)) {
        /* Move is two cells diagonally down */
        if (!same_colour(from_colour, cap_colour)) {
            /* Captured piece is the other player's */
            return 1;
        }
    }
    if (is_tower) {
        /* Tower */
        if (dist->row == -dir * 1 && (dist->col == 1 || dist->col == -1)) {
            /* Move is one cell diagonally up */
            return 1;
        }
        if (dist->row == -dir * 2 && (dist->col == 2 || dist->col == -2)) {
            /* Move is two cells diagonally up */
            if (!same_colour(from_colour, cap_colour)) {
                /* Captured piece is the other player's */
                return 1;
            }
        }
    }
    return 0;
}


/* Returns a coordinate diagonal to the one specified. dir from 1 to 4 is
 * top left, top right, bottom left, bottom right respectively. Doesn't check
 * if returned coord is outside the board. */
coord_t diag(coord_t from, int dist, int dir) {
    coord_t to;
    if (dir == 1) {
        /* Top left */
        to.row = from.row - dist;
        to.col = from.col - dist;
    } else if (dir == 2) {
        /* Top right */
        to.row = from.row - dist;
        to.col = from.col + dist;
    } else if (dir == 3) {
        /* Bottom left */
        to.row = from.row + dist;
        to.col = from.col - dist;
    } else {
        /* Bottom right */
        to.row = from.row + dist;
        to.col = from.col + dist;
    }
    return to;
}

/* Checks if two coordinates are the same */
int same_coord(coord_t *coord1, coord_t *coord2, coord_t *c_tested) {
    if (coord1->row == c_tested->row && coord1->col == c_tested->col ||
        coord2->row == c_tested->row && coord2->col == c_tested->col) {
        return 1;
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
/* THE END -------------------------------------------------------------------*/