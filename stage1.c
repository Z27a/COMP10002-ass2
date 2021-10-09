#include "ass2.h"

/* Stage 1 */
void do_stage1(board_t board, char turn) {
    printf("stage 1");
    /* initialise root */
    node_t *root;
    board_t *new_board;
    root = (node_t *) malloc(sizeof(node_t));
    new_board = (board_t *) malloc(sizeof(board_t));
    assert(root != NULL && new_board != NULL); //TODO: remember to have assertions. Disclose I used alistair's programs?
    root->board = new_board;
    board_cpy(board, *root->board);
    root->cost = get_cost(board);
    root->turn = turn;
    root->parent = NULL;
    list_t *handle;
    handle = (list_t *) malloc(sizeof(list_t));
    handle->head = handle->foot = NULL;
    root->child_handle = handle;

    // calculate possible moves and loop through them to run build tree

    build_tree(root, 1);


}


void build_tree(node_t *parent, int depth) {
    if (depth > TREE_DEPTH) {
        /* Base case ff*/
        parent->child_handle = NULL;
    } else {
        char prev_turn = switch_colour(parent->turn);
        node_t *new;
        moves_t moves;
        // get diags and then spam move_valid !!! :))))
        for (int row = 0; row < BOARD_SIZE; row++) {
            for (int col = 0; col < BOARD_SIZE; col++) {
                if (lower(*parent->board[row][col]) == parent->turn) {
                    /* Get possible moves */
                    moves = get_moves(row, col);
                    for (int i = 0; i < MAX_MOVES; i++) {
                        if (move_valid(*parent->board, &moves.moves[i], prev_turn, TRUE)) {
                            // malloc new node, update data
                            new = new_child(*parent->board, &parent->move, parent->turn);
                            // insert into child handle list
                            parent->child_handle = insert_child(parent->child_handle, new);
                            // run recursive function on that head
                            build_tree(parent->child_handle->foot->data, ++depth);
                        }
                    }
                }
            }
        }
    }
}

node_t *new_child(board_t prev_board, move_t *move, char prev_turn) {
    node_t *new;
    board_t *new_board;
    new = (node_t *) malloc(sizeof(node_t));
    new_board = (board_t *) malloc(sizeof(board_t));
    assert(new != NULL && new_board != NULL);
    new->board = new_board;
    board_cpy(prev_board, *new->board);
    update_board(*new->board, move);
    new->cost = get_cost(*new->board);
    move_cpy(move, &new->move);
    new->turn = switch_colour(prev_turn);
    return new;
}

list_t *insert_child(list_t *handle, node_t *data) {
    assert(handle != NULL);

    list_elem_t *new;
    new = (list_elem_t *) malloc(sizeof(list_elem_t));
    assert(new != NULL);
    new->next = NULL;
    new->data = data;

    if (handle->foot==NULL) {
        handle->foot = new;
    }
    return handle;
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

/* Returns a coordinate diagonal to the one specified. dir from 1 to 4 is
 * top left, top right, bottom left, bottom right respectively. Doesn't check
 * if returned coord is outside the board. */
moves_t get_moves(int row, int col) {
    moves_t moves;
    for (int i = 0; i < 2; i++) {
        /* NE */
        moves.moves[0 + i].to.row = row - (i + 1);
        moves.moves[0 + i].to.col = col + (i + 1);
    }
    for (int i = 0; i < 2; i++) {
        /* SE */
        moves.moves[2 + i].to.row = row + (i + 1);
        moves.moves[2 + i].to.col = col + (i + 1);
    }
    for (int i = 0; i < 2; i++) {
        /* SW */
        moves.moves[4 + i].to.row = row + (i + 1);
        moves.moves[4 + i].to.col = col - (i + 1);
    }
    for (int i = 0; i < 2; i++) {
        /* NW */
        moves.moves[6 + i].to.row = row - (i + 1);
        moves.moves[6 + i].to.col = col - (i + 1);
    }
    return moves;
}

/* Checks if two coordinates are the same */
int same_coord(coord_t *coord1, coord_t *coord2, coord_t *c_tested) {
    if (coord1->row == c_tested->row && coord1->col == c_tested->col ||
        coord2->row == c_tested->row && coord2->col == c_tested->col) {
        return 1;
    }
    return 0;
}