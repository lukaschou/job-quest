#ifndef DRAW_H
#define DRAW_H

#include "game_state.h"
#include <string.h>

#define MAX_OPTS 4
#define MAX_MSG_CHARS 256
#define STORE_ITEMS_WIDTH 50
#define STORE_DESC_WIDTH 70
#define STORE_DESC_HEIGHT 4

void draw(GameContext *ctx);
void draw_home(int rows, int cols, long apps);
void draw_store(int rows, int cols, GameContext *ctx);
void draw_opts(int rows, int cols, GameState state);
void create_box(
    int y,
    int x,
    int len,
    int height, 
    char corner,
    char horiz,
    char vert
);

#endif
