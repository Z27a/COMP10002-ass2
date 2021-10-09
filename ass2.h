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
#define NUM_PIECES (int) (ROWS_WITH_PIECES*BOARD_SIZE*0.5)

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

typedef struct {
    int action;
    char turn;
} next_action_t;

typedef struct node node_t;

struct node {
    board_t board;
    int cost;
    move_t move;
    char turn;
    node_t *parent;
    node_t *cdrn[TREE_DEPTH];
};


/* function prototypes -------------------------------------------------------*/
next_action_t do_stage0(board_t board);

void do_stage1(board_t board, char turn);

node_t *build_tree(node_t *node, int depth);

void board_cpy(board_t orig, board_t new);

void do_stage2(board_t board);

void init_board(board_t board);

void fill_pieces(int row_even, int row, board_t board, char piece);

void prt_board(board_t board);

next_action_t prt_from_input(board_t board);

void move_valid(board_t board, move_t *move, char prev_turn);

int outside_board(coord_t *coord);

int cell_empty(board_t board, coord_t *coord);

int same_colour(char c1, char c2);

char lower(char c);

char upper(char c);

int is_upper(char c);

int legal_action(board_t board, move_t *move, char from_piece);

int valid_move(int dir, coord_t *dist, char from_colour, char cap_colour,
               int is_tower);

coord_t get_dist(move_t *move);

void update_board(board_t board, move_t *move);

int get_cost(board_t board);

coord_t diag(coord_t from, int dist, int dir);

int same_coord(coord_t *coord1, coord_t *coord2, coord_t *c_tested);

void pad();

void newline();