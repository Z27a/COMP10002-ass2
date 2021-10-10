#include "ass2.h"

/* Stage 1 */
void do_stage1(board_t board, char turn) {
    printf("stage 1\n");
    /* initialise root */
    state_t *root;
    root = init_root(board, turn);
    // calculate possible moves and loop through them to run build tree
    build_tree(root, 1);
}


void build_tree(state_t *parent, int depth) {
    if (depth > TREE_DEPTH) {
        /* Base case */
        return;
    } else {
        char prev_turn = switch_colour(parent->turn);
        state_t *new;
        move_ary psbl_mvs;
        for (int row = 0; row < BOARD_SIZE; row++) {
            for (int col = 0; col < BOARD_SIZE; col++) {
                if (lower(parent->board[row][col]) == parent->turn) {
                    /* Get possible psbl_mvs */
                    psbl_mvs = get_moves(row, col);
                    for (int i = 0; i < MAX_MOVES; i++) {
                        if (move_valid(parent->board, &psbl_mvs.moves[i], prev_turn,
                                       TRUE)) {
                            // malloc new state, update state
                            new = new_child(parent->board, &psbl_mvs.moves[i],
                                            parent->turn);
                            // insert into child handle list
                            insert_child(parent->child_handle, new);
                            // run recursive function on that head
                            build_tree(parent->child_handle->foot->state, ++depth);
                        }
                    }
                }
            }
        }
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
    new->turn = switch_colour(prev_turn);
    new->child_handle = new_handle();
    return new;
}

void insert_child(list_t *handle, state_t *data) {
    assert(handle != NULL);
    lst_node_t *new;
    new = (lst_node_t *) malloc(sizeof(lst_node_t));
    assert(new != NULL);
    new->next = NULL;
    new->state = data;

    if (handle->foot == NULL) {
        /* First insertion */
        handle->head = handle->foot = new;
    } else {
        handle->foot->next = new;
        handle->foot = new;
    }
}

state_t *init_root(board_t board, char turn) {
    state_t *root;
    root = (state_t *) malloc(sizeof(state_t));
    assert(root != NULL);
    //TODO: remember to have assertions. Disclose I used alistair's programs?
    board_cpy(board, root->board);
    root->cost = get_cost(board);
    root->turn = turn;
    root->parent = NULL;
    root->child_handle = new_handle();
    return root;
}

list_t *new_handle() {
    list_t *handle;
    handle = (list_t *) malloc(sizeof(list_t));
    assert(handle!=NULL);
    handle->head = handle->foot = NULL;
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
