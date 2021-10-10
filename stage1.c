#include "ass2.h"

/* Stage 1 */
void do_stage1(board_t board, char prev_turn) {
    printf("stage 1\n");
    /* initialise root */
    state_t *root;
    /* Insert previous turn into root because the root is the built from the
     * state of the game where the previous player had just played */
    root = init_root(board, switch_colour(prev_turn));
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
    prt_move(cam.move);
    //printf("curr_turn: %c\n", root->cur_turn);
    printf("cost: %d\n", cam.cost);
    printf("depth: %d\n", cam.depth);

}


void build_tree(state_t *parent, int depth) {
    if (depth > TREE_DEPTH) {
        /* Base case */
        return;
    } else {
        char prev_turn = switch_colour(parent->cur_turn);
        state_t *new;
        move_ary psbl_mvs;
        int piece_found = FALSE;
        for (int row = 0; row < BOARD_SIZE; row++) {
            for (int col = 0; col < BOARD_SIZE; col++) {
                if (lower(parent->board[row][col]) == parent->cur_turn) {
                    piece_found = TRUE;
                    /* Get possible moves */
                    psbl_mvs = get_moves(row, col);
                    for (int i = 0; i < MAX_MOVES; i++) {
                        if (move_valid(parent->board, &psbl_mvs.moves[i], prev_turn,
                                       TRUE)) {
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

void insert_child(list_t *handle, state_t *data) {
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
    root->cost = get_cost(board);
    root->cur_turn = cur_turn;
    root->child_hdl = new_handle();
    root->move.from.row = -1;
    root->depth = 0;

    return root;
}

list_t *new_handle() {
    list_t *handle;
    handle = (list_t *) malloc(sizeof(list_t));
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
            cam.depth = state->depth;
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


void prt_move(move_t move) {
    char m1, m2, m3, m4;
    m1 = move.from.col + 'A';
    m2 = move.from.row + '0' + 1;
    m3 = move.to.col + 'A';
    m4 = move.to.row + '0' + 1;
    printf("next move: %c%c-%c%c\n", m1, m2, m3, m4);
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
