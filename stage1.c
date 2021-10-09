#include "ass2.h"

/* Stage 1 */
void do_stage1(board_t board, char turn) {
    printf("stage 1");
    /* initialise root */
    node_t *root;
    root = (node_t *) malloc(sizeof(node_t));
    assert(root != NULL);

    board_cpy(board, root->board);
    root->cost = get_cost(board);
    root->turn = turn;

    build_tree(root, 1);


}

node_t *build_tree(node_t *node, int depth) {
    node_t *new_node = NULL;
    if (depth > TREE_DEPTH) {
        /* Base case */
        return NULL;
    } else {
        // calculate possible moves
        // create child with possible move
        // update board for it
        // update cost for it
        // run algo on childs?
    }

    return new_node;
}

/* Copies the contents of one board onto another */
void board_cpy(board_t orig, board_t new) {
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            new[row][col] = orig[row][col];
        }
    }
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