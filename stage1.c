#include "ass2.h"

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