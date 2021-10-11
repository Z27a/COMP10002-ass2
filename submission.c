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
#define MAX_MOVES 8

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
    move_t moves[MAX_MOVES];
} move_ary;

typedef struct {
    int action;
    char prev_turn;
    int num_turns;
} nxt_act_t;

typedef struct lst_node lst_node_t;
typedef struct state state_t;

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
    int depth;
};

typedef struct {
    int cost;
    move_t move;
    int depth;
} cam_t;


/* Stage 0 ################################################################## */
void do_stage0(board_t board, nxt_act_t *nxt_act);

void init_board(board_t board);

void fill_pieces(int row_even, int row, board_t board, char piece);

void prt_board(board_t board);

void prt_from_input(board_t board, nxt_act_t *nxt_act);

void
prt_inb(board_t board, nxt_act_t *nxt_act, move_t *move, int cost, int is_s1);

int move_valid(board_t board, move_t *move, char prev_turn, int not_exit);

int outside_board(coord_t *coord);

int cell_empty(board_t board, coord_t *coord);

int same_colour(char c1, char c2);

char lower(char c);

char upper(char c);

int is_upper(char c);

int legal_action(board_t board, move_t *move, char from_piece);

coord_t get_dist(move_t *move);

void update_board(board_t board, move_t *move);

int get_cost(board_t board);

int check_win(board_t board, char cur_turn);

move_ary get_moves(int row, int col);

void pad();

void newline();

/* Stage 1 ################################################################## */
void do_stage1(board_t board, nxt_act_t *nxt_act);

void build_tree(state_t *parent, int depth);

char switch_colour(char orig);

void move_cpy(move_t *orig, move_t *new);

state_t *new_child(board_t prev_board, move_t *move, char prev_turn);

void insert_child(lst_t *handle, state_t *data);

state_t *init_root(board_t board, char cur_turn);

lst_t *new_handle();

void board_cpy(board_t orig, board_t new);

cam_t backprop_cost(state_t *state, char order);

void prt_move(move_t *move);

void free_tree(state_t *parent);

void do_stage2(board_t board, nxt_act_t *nxt_act);



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
        do_stage2(board, &next);
    } else {
        /* check for winner */
    }


    return EXIT_SUCCESS;            // exit program with the success code
}

/* THE END -------------------------------------------------------------------*/



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



/* Stage 1 */
void do_stage1(board_t board, nxt_act_t *nxt_act) {
    // TODO: if prev_turn == '\0' make it black?
    /* initialise root */
    state_t *root;
    /* Insert previous prev_turn into root because the root is the built from the
     * state of the game where the previous player had just played */
    root = init_root(board, switch_colour(nxt_act->prev_turn));
    // calculate possible moves and loop through them to run build tree
//    printf("prev_turn: %c", prev_turn);
//    move_ary psbl_mvs = get_moves(5, 2);
//    for (int i = 0; i < MAX_MOVES; i++) {
//        prt_move(psbl_mvs.moves[i]);
//        if (move_valid(root->board, &psbl_mvs.moves[i], prev_turn,
//                       TRUE)) {
//            printf("valid\n");
//        }
//    }

    build_tree(root, 1);

    cam_t cam = backprop_cost(root, root->cur_turn);

    free_tree(root);

    update_board(board, &cam.move);

    prt_inb(board, nxt_act, &cam.move, get_cost(board), TRUE);

    nxt_act->num_turns++;
}


void build_tree(state_t *parent, int depth) {
    if (depth > TREE_DEPTH) {
        /* Base case */
        return;
    } else {
        char prev_turn = switch_colour(parent->cur_turn);
        state_t *new;
        move_ary psbl_mvs;
//        int piece_found = FALSE;
//        int valid_mv_found = FALSE;
        for (int row = 0; row < BOARD_SIZE; row++) {
            for (int col = 0; col < BOARD_SIZE; col++) {
                if (lower(parent->board[row][col]) == parent->cur_turn) {
//                    piece_found = TRUE;
                    /* Get possible moves */
                    psbl_mvs = get_moves(row, col);
                    for (int i = 0; i < MAX_MOVES; i++) {
                        if (move_valid(parent->board, &psbl_mvs.moves[i], prev_turn,
                                       TRUE)) {
//                            valid_mv_found = TRUE;
                            // malloc new data, update data
                            new = new_child(parent->board, &psbl_mvs.moves[i],
                                            parent->cur_turn);
                            new->depth = depth;
                            // insert into child handle list
                            insert_child(parent->child_hdl, new);
                            // run recursive function on the new child
                            build_tree(new, depth + 1);
                        }
                    }
                }
            }
        }
//        if (piece_found == FALSE || (piece_found = TRUE && valid_mv_found == FALSE)) {
//            /* Winning condition */
//            if (parent->cur_turn == CELL_WPIECE) {
//                parent->cost = INT_MIN;
//            } else {
//                parent->cost = INT_MAX;
//            }
//        }
    }
}

void free_tree(state_t *parent) {
    assert(parent != NULL);
    if (parent->child_hdl->head == NULL) {
        /* Base case - no more children to free. */
        free(parent->child_hdl);
        free(parent);
        return;
    } else {
        /* Loop through children and free them. This section is structured
         * similarly to Alistair's code */
        assert(parent->child_hdl != NULL);
        lst_node_t *curr, *prev;
        curr = parent->child_hdl->head;
        while (curr) {
            prev = curr;
            curr = curr->next;
            free_tree(prev->data);
            free(prev);
        }
        free(parent->child_hdl);
        free(parent);
    }
}

state_t *new_child(board_t prev_board, move_t *move, char prev_turn) {
    state_t *new;
    new = (state_t *) malloc(sizeof(state_t));
    assert(new != NULL);
    board_cpy(prev_board, new->board);
    update_board(new->board, move);
    new->cost = get_cost(new->board);
    move_cpy(move, &new->move);
    new->cur_turn = switch_colour(prev_turn);
    new->child_hdl = new_handle();
    return new;
}

void insert_child(lst_t *handle, state_t *data) {
    assert(handle != NULL);
    lst_node_t *new;
    new = (lst_node_t *) malloc(sizeof(lst_node_t));
    assert(new != NULL);
    new->next = NULL;
    new->data = data;

    if (handle->foot == NULL) {
        /* First insertion */
        handle->head = handle->foot = new;
    } else {
        handle->foot->next = new;
        handle->foot = new;
    }
}

state_t *init_root(board_t board, char cur_turn) {
    state_t *root;
    root = (state_t *) malloc(sizeof(state_t));
    assert(root != NULL);
    //TODO: remember to have assertions. Disclose I used alistair's programs?
    board_cpy(board, root->board);
    root->cost = get_cost(board );
    root->cur_turn = cur_turn;
    root->child_hdl = new_handle();
    root->move.from.row = -1;
    root->depth = 0;
    return root;
}

lst_t *new_handle() {
    lst_t *handle;
    handle = (lst_t *) malloc(sizeof(lst_t));
    assert(handle!=NULL);
    handle->head = handle->foot = NULL;
    return handle;
}


cam_t backprop_cost(state_t *state, char order) {
    cam_t cam;
    if (state->child_hdl->head == NULL) {
        /* Leaf node, no more children, base case. Return cost of current
         * leaf node*/
        cam.cost = state->cost;
        cam.move = state->move;
        cam.depth = state->depth;
    } else {
        /* Recursive case. Return max or min cost of child states */
        cam = backprop_cost(state->child_hdl->head->data, switch_colour(order));
        if (state->move.from.row != -1) {
            /* If current state is not the root, return the move and depth of
             * the current state */
            cam.move = state->move;
            //cam.depth = state->depth;
        } else {
            /* The current state is the root. Return the move of the min/max
             * cost child state */
            cam.move = state->child_hdl->head->data->move;
        }
        cam_t temp_cam;
        /* pointer to the first next element in the list */
        lst_node_t *next_elem = state->child_hdl->head->next;
        while (next_elem != NULL) {
            temp_cam = backprop_cost(next_elem->data, switch_colour(order));
            if (order == CELL_WPIECE) {
                /* Minimise cost, Update cam with lower cost */
                if (cam.cost > temp_cam.cost) {
                    cam.cost = temp_cam.cost;
                    cam.depth = temp_cam.depth;
                    if (state->move.from.row != -1) {
                        cam.move = state->move;
                    } else {
                        cam.move = next_elem->data->move;
                    }
                }
            } else {
                /* Maximise cost */
                if (cam.cost < temp_cam.cost) {
                    cam.cost = temp_cam.cost;
                    cam.depth = temp_cam.depth;
                    if (state->move.from.row != -1) {
                        cam.move = state->move;
                    } else {
                        cam.move = next_elem->data->move;
                    }
                }
            }
            next_elem = next_elem->next;
        }
    }
    return cam;
}


void prt_move(move_t *move) {
    char m1, m2, m3, m4;
    m1 = move->from.col + 'A';
    m2 = move->from.row + '0' + 1;
    m3 = move->to.col + 'A';
    m4 = move->to.row + '0' + 1;
    printf("%c%c-%c%c\n", m1, m2, m3, m4);
//    printf("col-row: %d,%d-%d,%d\n", move.from.col, move.from.row,
//           move.to.col, move.to.row );
}

/* Copies the contents of one board onto another */
void board_cpy(board_t orig, board_t new) {
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            new[row][col] = orig[row][col];
        }
    }
}

void move_cpy(move_t *orig, move_t *new) {
    new->to = orig->to;
    new->from = orig->from;
}

char switch_colour(char orig) {
    char new;
    if (orig == CELL_WPIECE) {
        new = CELL_BPIECE;
    } else {
        new = CELL_WPIECE;
    }
    return new;
}

move_ary get_moves(int row, int col) {
    move_ary new;
    for (int i = 0; i < MAX_MOVES; i++) {
        new.moves[i].from.row = row;
        new.moves[i].from.col = col;
    }
    for (int i = 0; i < 2; i++) {
        /* NE */
        new.moves[0 + i].to.row = row - (i + 1);
        new.moves[0 + i].to.col = col + (i + 1);
    }
    for (int i = 0; i < 2; i++) {
        /* SE */
        new.moves[2 + i].to.row = row + (i + 1);
        new.moves[2 + i].to.col = col + (i + 1);
    }
    for (int i = 0; i < 2; i++) {
        /* SW */
        new.moves[4 + i].to.row = row + (i + 1);
        new.moves[4 + i].to.col = col - (i + 1);
    }
    for (int i = 0; i < 2; i++) {
        /* NW */
        new.moves[6 + i].to.row = row - (i + 1);
        new.moves[6 + i].to.col = col - (i + 1);
    }
    return new;
}

void do_stage2(board_t board, nxt_act_t *nxt_act) {
    for (int i = 0; i < 10; i++) {
        do_stage1(board, nxt_act);
    }
}
