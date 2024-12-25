#include "state.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "snake_utils.h"

/* Helper function definitions */
// static void set_board_at(game_state_t *state, unsigned int row, unsigned int col, char ch);
static bool is_tail(char c);
static bool is_head(char c);
static bool is_snake(char c);
static char body_to_tail(char c);
static char head_to_body(char c);
static unsigned int get_next_row(unsigned int cur_row, char c);
static unsigned int get_next_col(unsigned int cur_col, char c);
static void find_head(game_state_t *state, unsigned int snum);
static char next_square(game_state_t *state, unsigned int snum);
static void update_tail(game_state_t *state, unsigned int snum);
static void update_head(game_state_t *state, unsigned int snum);

/* Task 1 */
game_state_t *create_default_state() {
    game_state_t* game = malloc(sizeof(game_state_t));
    // game->num_rows = (unsigned int*) malloc(sizeof(unsigned int));
    game->num_rows = 18;
    game->board = malloc(sizeof(char*) * 18);
    for (int i = 0; i < 18; i++) {
        game->board[i] = malloc(sizeof(char) * 20 + 1 + 1); // plus \n plus \0
        if ((i == 0) || (i == 17)) {
            //game->board[i][20] = '\0'; 
            for (int j = 0; j <= 19; j++) {
                game->board[i][j] = '#';
            }
            game->board[i][20] = '\n';
            game->board[i][21] = '\0';
        } else { // break
            //game->board[i][20] = '\0'; 
            for (int j = 0; j <= 19; j++) {
                if (j == 0 || j == 19) {
                    game->board[i][j] = '#';
                } else if (i == 2 && j == 9) {
                    game->board[i][j] = '*';
                } else if (i == 2 && j == 2) {
                    game->board[i][j] = 'd';
                } else if (i == 2 && j == 3) {
                    game->board[i][j] = '>';
                } else if (i == 2 && j == 4) {
                    game->board[i][j] = 'D';
                } else {
                    game->board[i][j] = ' ';
                }
            }
            game->board[i][20] = '\n';
            game->board[i][21] = '\0';
        }
        // game->board[i][20] = '\0'; // probably doesnt work as it should
    }

    game->num_snakes = 1;
    game->snakes = malloc(sizeof(snake_t) * 4);

    snake_t* snake;
    snake = malloc(sizeof(snake_t));
    //snake->tail_row = malloc(sizeof(int));
    snake->tail_row = 2;
    //snake->tail_col = malloc(sizeof(int));
    snake->tail_col = 2;
    //snake->head_row = malloc(sizeof(int));
    snake->head_row = 2;
    //snake->head_col = malloc(sizeof(int));
    snake->head_col = 4;
    //snake->live = malloc(sizeof(bool));
    snake->live = true;
    game->snakes[0] = *snake;
    return game;
}

/* Task 2 */
void free_state(game_state_t *state) {
    // for (int i = 0; i < 18; i++) {
    for (int i = 0; i < state->num_rows; i++) {
         free(state->board[i]);
    }
    free(state->board);
    free(state->snakes);
    free(state);
    return;
}

/* Task 3 */
void print_board(game_state_t *state, FILE *fp) {
    for (unsigned int i = 0; i < state->num_rows; i++) {
        // for (int j = 0; j < 20; j++) {
        // for (int j = 0; j < strlen(state->board[i]); j++) {
        //     fprintf(fp, "%c", state->board[i][j]);
        // }
        // fprintf(fp, "\n");
        // fprintf(fp, "%c", '\n');
        fprintf(fp, "%s", state->board[i]);
    }


    //fprintf(fp, "%s", state->board[17]);
    //fprintf(fp, "%s\n", state->board[5]);
    //
    //for (int i = 0; i < 17; i++) {
    //     // for (int c=0; c < 20; c++) {
    //     //    fprintf(fp, state.board[r][c]);
    //     //}
         //fprintf(fp, "%s\n", state->board[i]);
    // }
    //fprintf(fp, "%s", state->board[17]);
    //fprintf(fp, "%s\n", state->board[5]);
    return;
}

/*
  Saves the current state into filename. Does not modify the state object.
  (already implemented for you).
*/
void save_board(game_state_t *state, char *filename) {
  FILE *f = fopen(filename, "w");
  print_board(state, f);
  fclose(f);
}

/* Task 4.1 */

/*
  Helper function to get a character from the board
  (already implemented for you).
*/
char get_board_at(game_state_t *state, unsigned int row, unsigned int col) { return state->board[row][col]; }

/*
  Helper function to set a character on the board
  (already implemented for you).
*/
static void set_board_at(game_state_t *state, unsigned int row, unsigned int col, char ch) {
  state->board[row][col] = ch;
}

/*
  Returns true if c is part of the snake's tail.
  The snake consists of these characters: "wasd"
  Returns false otherwise.
*/
static bool is_tail(char c) {
    if (c == 'w' || c == 'a' || c == 's' || c == 'd') {
        return true;
    }
    return false;
}

/*
  Returns true if c is part of the snake's head.
  The snake consists of these characters: "WASDx"
  Returns false otherwise.
*/
static bool is_head(char c) {
    if (c == 'W' || c == 'A' || c == 'S' || c == 'D' || c == 'x') {
        return true;
    }
    return false;
}

/*
  Returns true if c is part of the snake.
  The snake consists of these characters: "wasd^<v>WASDx"
*/
static bool is_snake(char c) {
    if (c == 'w' || c == 'a' || c == 's' || c == 'd'
            || c == 'W' || c == 'A' || c == 'S' || c == 'D'
            || c == '^' || c == '<' || c == '>' || c == 'v'
            || c == 'x') {
        return true;
    }
    return false;
}

/*
  Converts a character in the snake's body ("^<v>")
  to the matching character representing the snake's
  tail ("wasd").
*/
static char body_to_tail(char c) {
    if (c == '^') {return 'w';}
    if (c == '<') {return 'a';}
    if (c == 'v') {return 's';}
    if (c == '>') {return 'd';}
    return '?';
}

/*
  Converts a character in the snake's head ("WASD")
  to the matching character representing the snake's
  body ("^<v>").
*/
static char head_to_body(char c) {
    if (c == 'W') {return '^';}
    if (c == 'A') {return '<';}
    if (c == 'S') {return 'v';}
    if (c == 'D') {return '>';}
    return '?';
}

/*
  Returns cur_row + 1 if c is 'v' or 's' or 'S'.
  Returns cur_row - 1 if c is '^' or 'w' or 'W'.
  Returns cur_row otherwise.
*/
static unsigned int get_next_row(unsigned int cur_row, char c) {
    if (c == 'v' || c == 's' || c == 'S') {
        return cur_row + 1;
    }
    if (c == '^' || c == 'w' || c == 'W') {
        return cur_row - 1;
    }
    else {
        return cur_row;
    }
}

/*
  Returns cur_col + 1 if c is '>' or 'd' or 'D'.
  Returns cur_col - 1 if c is '<' or 'a' or 'A'.
  Returns cur_col otherwise.
*/
static unsigned int get_next_col(unsigned int cur_col, char c) {
    if (c == '>' || c == 'd' || c == 'D') {
        return cur_col + 1;
    }
    if (c == '<' || c == 'a' || c == 'A') {
        return cur_col - 1;
    }
    else {
        return cur_col;
    }
}

/*
  Task 4.2

  Helper function for update_state. Return the character in the cell the snake is moving into.

  This function should not modify anything.
*/
/* helpers */
void set_snake_head(game_state_t* state, unsigned int snum,
        unsigned int row, unsigned int col) {
    if (snum > (state->num_snakes - 1)) {
        return;
    }

    state->snakes[snum].head_row = row;
    state->snakes[snum].head_col = col;
}
void set_snake_tail(game_state_t* state, unsigned int snum,
        unsigned int row, unsigned int col) {
    if (snum > (state->num_snakes - 1)) {
        return;
    }

    state->snakes[snum].tail_row = row;
    state->snakes[snum].tail_col = col;
}
bool is_snum_head(game_state_t* state, unsigned int snum,
        unsigned int row, unsigned int col) {
    if (snum > (state->num_snakes - 1)) {
        return false;
    }

    return state->snakes[snum].head_row == row && state->snakes[snum].head_col == col;
}
bool is_snum_tail(game_state_t* state, unsigned int snum,
        unsigned int row, unsigned int col) {
    if (snum > (state->num_snakes - 1)) {
        return false;
    }

    return state->snakes[snum].tail_row == row && state->snakes[snum].tail_col == col;
}

/* task */
static char next_square(game_state_t *state, unsigned int snum) {
    for (unsigned int i=0; i < state->num_rows; i++) {
        for (unsigned int j=0; j < strlen(state->board[i]); j++) {
            // is_snum_head is only defined after this line
            if (is_head(get_board_at(state, i, j)) && is_snum_head(state, snum, i, j)) {
                return state->board[get_next_row(i, get_board_at(state, i, j))]
                   [get_next_col(j, get_board_at(state, i, j))];
            }
        }
    }
    return '0';
}

/*
  Task 4.3

  Helper function for update_state. Update the head...

  ...on the board: add a character where the snake is moving

  ...in the snake struct: update the row and col of the head

  Note that this function ignores food, walls, and snake bodies when moving the head.
*/

static void update_head(game_state_t *state, unsigned int snum) {
    // unsigned int i_temp;
    // unsigned int j_temp;
    // for (unsigned int i=0; i < state->num_rows; i++) {
    //     for (unsigned int j=0; j < strlen(state->board[i]); j++) {
    //         if (is_head(state->board[i][j]) && state->snakes[snum].head_row == i && state->snakes[snum].head_col == j) {
    //             state->board[get_next_row(i, state->board[i][j])][get_next_col(j, state->board[i][j])] = state->board[i][j];
    //             state->board[i][j] = head_to_body(state->board[i][j]);
    //             i_temp = i;
    //             j_temp = j;
    //         }
    //     }
    // }
    // state->snakes[snum].head_row = get_next_row(i_temp, state->board[i_temp][j_temp]);
    // state->snakes[snum].head_col = get_next_col(j_temp, state->board[i_temp][j_temp]);

    // redo 6/28/24 2:48pm
    for (unsigned int i=0; i < state->num_rows; i++) {
        for (unsigned int j=0; j < strlen(state->board[i]); j++) {
            char cursor = get_board_at(state, i, j);
            if (is_head(cursor) && is_snum_head(state, snum, i, j)) {
                set_board_at(state, get_next_row(i, cursor),
                        get_next_col(j, cursor), cursor);

                set_board_at(state, i, j, head_to_body(cursor));

                set_snake_head(state, snum,
                        get_next_row(i, cursor), get_next_col(j, cursor));
                return;
            }
        }
    }
}

/*
  Task 4.4

  Helper function for update_state. Update the tail...

  ...on the board: blank out the current tail, and change the new
  tail from a body character (^<v>) into a tail character (wasd)

  ...in the snake struct: update the row and col of the tail
*/
static void update_tail(game_state_t *state, unsigned int snum) {
    // unsigned int i_temp;
    // unsigned int j_temp;
    // for (unsigned int i=0; i < state->num_rows; i++) {
    //     for (unsigned int j=0; j < strlen(state->board[i]); j++) {
    //         if (is_tail(state->board[i][j])  && state->snakes[snum].tail_row == i && state->snakes[snum].tail_col == j) {
    //             state->board[get_next_row(i, state->board[i][j])][get_next_col(j, state->board[i][j])] = state->board[i][j];
    //             state->board[i][j] = ' ';
    //             i_temp = i;
    //             j_temp = j;
    //         }
    //     }
    // }
    // state->snakes[snum].tail_row = get_next_row(i_temp, state->board[i_temp][j_temp]);
    // state->snakes[snum].tail_col = get_next_col(j_temp, state->board[i_temp][j_temp]);

    // redo 6/28/24 3:20pm
    if (snum > (state->num_snakes - 1)) {
        return;
    }

    for (unsigned int i=0; i < state->num_rows; i++) {
        for (unsigned int j=0; j < strlen(state->board[i]); j++) {
            char cursor = get_board_at(state, i, j);
            char ahead = get_board_at(state, get_next_row(i, cursor),
                    get_next_col(j, cursor));
            if (is_tail(cursor) && is_snum_tail(state, snum, i, j)) {
                set_board_at(state, get_next_row(i, cursor),
                        get_next_col(j, cursor), body_to_tail(ahead));
                set_board_at(state, i, j, ' ');
                set_snake_tail(state, snum,
                        get_next_row(i, cursor), get_next_col(j, cursor));
                return;
            }
        }
    }
}

/* Task 4.5 */

/* helpers */
unsigned int get_snum_head_row(game_state_t* state, unsigned int snum) {
    if (snum > (state->num_snakes - 1)) {
        return '0';
    }

    return state->snakes[snum].head_row;
}
unsigned int get_snum_head_col(game_state_t* state, unsigned int snum) {
    if (snum > (state->num_snakes - 1)) {
        return '0';
    }

    return state->snakes[snum].head_col;
}
char get_snum_head_char(game_state_t* state, unsigned int snum) {
    if (snum > (state->num_snakes - 1)) {
        return '0';
    }

    snake_t* the_snake = malloc(sizeof(snake_t));
    *the_snake = state->snakes[snum];

    unsigned int* row = malloc(sizeof(unsigned int));
    *row = the_snake->head_row;

    unsigned int* col = malloc(sizeof(unsigned int));
    *col = the_snake->head_col;

    return get_board_at(state, *row, *col);
}

/* task */
void update_state(game_state_t *state, int (*add_food)(game_state_t *state)) {
    for (unsigned int i=0; i < state->num_snakes; i++) {
        char* snum_head_char = malloc(sizeof(char));
        *snum_head_char = get_snum_head_char(state, i);

        unsigned int next_row = get_next_row(get_snum_head_row(state, i), *snum_head_char);
        unsigned int next_col = get_next_col(get_snum_head_col(state, i), *snum_head_char);

        unsigned int* s_head_row = malloc(sizeof(unsigned int));
        *s_head_row = get_snum_head_row(state, i);

        unsigned int* s_head_col = malloc(sizeof(unsigned int));
        *s_head_col = get_snum_head_col(state, i);

        if (get_snum_head_row(state, i) > 0 && get_snum_head_col(state, i) > 0) {
            if (get_board_at(state, next_row, next_col) == '#'
                    || is_snake(get_board_at(state, next_row, next_col))) {
                set_board_at(state, *s_head_row, *s_head_col, 'x');
                state->snakes[i].live = false;
            }

            else if (get_board_at(state, next_row, next_col) == '*') {
                add_food(state);
                update_head(state, i);
            }

            else {
                update_head(state, i);
                update_tail(state, i);
            }
        }

    }
}

/* Task 5.1 */
char *read_line(FILE *fp) {
    // FILE* fp_temp = fp;
    //
    // unsigned int i = 1;
    // char* test = malloc(i);
    // while (fgets(test, sizeof(test), fp_temp) != NULL) {
    //     test = realloc(test, i++);
    //     fp_temp = fp;
    // }
    // //
    // //
    // // char* temp1 = malloc(sizeof(char));
    // char* temp1 = malloc(sizeof(test));
    // fgets(temp1, sizeof(temp1), fp);
    // if (temp1 == NULL) {
    //     return NULL;
    // };
    // return temp1;
    
    // attempt 2
    // int n = 1;
    // char* static_temp[n];
    // do {
    //     n++;
    //     char* static_temp[n];
    //     fgets(static_temp, n, fp);
    // } while (strchr(static_temp, '\0') == NULL);

    // attempt 3
    // char* line = '\0';

    // unsigned int total_len = 0;
    // char buffer[1+1];

    // while (fgets(buffer, 1+1, fp) != NULL) {
    //     char* new_line = realloc(line, total_len + (1 + 1)); // new character buffer + null terminator

    //     line = new_line;
    //     strcpy(line + total_len, buffer);
    //     total_len++;

    //     if (buffer[0] == '\n') {
    //         break;
    //     }
    // }

    // return line;

    // attempt 4
    char* line = malloc(1); // just null terminator, empty string
    line[0] = '\0';
    char buffer[sizeof(char)+1]; // one character from file + null terminator
    unsigned int total_len = 0;

    while (fgets(buffer, sizeof(buffer), fp) != NULL) { // fgets n needs to include null terminator
        char* new_line = realloc(line, total_len +sizeof(buffer));
        if (new_line == NULL) {
            free(line);
            return NULL;
        }
        line = new_line;

        // strcpy(line + total_len, buffer);
        line[total_len] = buffer[0];

        total_len++;
        line[total_len] = '\0';

        if (buffer[0] == '\n') {break;}
    }

    // if (buffer[0] == '\0') {return NULL;} // EOF
    if (total_len == 0) { // EOF
        free(line);
        return NULL;
    }
    return line;
}

/* Task 5.2 */
game_state_t *load_board(FILE *fp) {
    game_state_t* game = malloc(sizeof(game_state_t));
    game->num_rows = 0;
    unsigned int index = 0;
    // char* temp_row = malloc(sizeof(char*));
    // temp_row = read_line(fp);
    char* temp_row = read_line(fp); // no need to malloc temp_row
    // game->board = malloc(sizeof(char**)); // i think char** is wrong
    game->board = malloc(sizeof(char*));

    while (temp_row != NULL) {
        // game->board[index] = malloc(strlen(temp_row+1)); // +1 null terminator
        game->board[index] = temp_row;
        // strcpy(game->board[index], temp_row); // using strcpy instead
        // free(temp_row); // freed after copying to game->board[index]

        game->num_rows += 1;
        index += 1;

        game->board = realloc(game->board, (index+1) * sizeof(char*));
        temp_row = read_line(fp);
    }
    // might need to remove the last realloc'ed char* once we hit EOF
    // game->num_rows += 1; // why +1 again?
    game->num_snakes = 0;
    game->snakes = NULL;
    return game;
}

/*
  Task 6.1

  Helper function for initialize_snakes.
  Given a snake struct with the tail row and col filled in,
  trace through the board to find the head row and col, and
  fill in the head row and col in the struct.
*/
/* helper */
bool cursor_is_not_head(char cursor) {
    return cursor != 'W' && cursor != 'A' && cursor != 'S' && cursor != 'D';
}
// gave up this helper because of pass by value
// void follow_snum_tail_to_head(game_state_t* state, unsigned int snum, unsigned int tail_row, unsigned int tail_col) {
//     char cursor;
//     do {
//         cursor = get_board_at(state, cursor_row, cursor_col);
//     } while (cursor_is_not_head(cursor))
// }

/* task */
static void find_head(game_state_t *state, unsigned int snum) {
    if (snum >= (state->num_snakes)) {
        return;
    }

    unsigned int cursor_row = state->snakes[snum].tail_row;
    unsigned int cursor_col = state->snakes[snum].tail_col;
    char cursor = get_board_at(state, cursor_row, cursor_col);

    while (cursor_is_not_head(cursor)) {
        cursor_row = get_next_row(cursor_row, cursor);
        cursor_col = get_next_col(cursor_col, cursor);
        cursor = get_board_at(state, cursor_row, cursor_col);
    }

    // do {
    //     cursor = get_board_at(state, cursor_row, cursor_col);
    //     cursor_row = get_next_row(cursor_row, cursor);
    //     cursor_col = get_next_col(cursor_col, cursor);
    // } while (cursor_is_not_head(cursor));


    state->snakes[snum].head_row = cursor_row;
    state->snakes[snum].head_col = cursor_col;
}

/* Task 6.2 */
game_state_t *initialize_snakes(game_state_t *state) {
    char cursor;
    unsigned int i = 0;
    for (unsigned int row = 0; row < state->num_rows; row++) {
        for (unsigned int col = 0; col < strlen(state->board[row]); col++) {
            cursor = get_board_at(state, row, col);

            if (is_tail(cursor)) {
                i++;
            }
        }
    }

    state->num_snakes = i;
    state->snakes = malloc(sizeof(snake_t) * i);
    unsigned int j = 0;
    for (unsigned int row = 0; row < state->num_rows; row++) {
        for (unsigned int col = 0; col < strlen(state->board[row]); col++) {
            cursor = get_board_at(state, row, col);

            if (is_tail(cursor)) {
                state->snakes[j].tail_row = row;
                state->snakes[j].tail_col = col;
                find_head(state, j);
                state->snakes[j].live = true;
                j++;
            }
        }
    }

    return state;
}
