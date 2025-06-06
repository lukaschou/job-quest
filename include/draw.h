#ifndef DRAW_H
#define DRAW_H

#include "game_state.h"
#include <string.h>

#define SUFFIX_THRESHOLD 1000000
#define MAX_SUFFIX_FORMAT_LEN 10
#define MIN_STORE_BOX_WIDTH 50
#define MAX_STORE_BOX_WIDTH 80
#define MAX_APPS_DISPLAY_LEN (MIN_STORE_BOX_WIDTH) 
#define SMALL_SCR_HEIGHT_THRESH 45
#define ITEM_DESC_BOX_HEIGHT 6

int store_header_start_row(int scr_rows);
int opts_start_row(int scr_rows);
int store_items_box_width(int scr_cols);
int text_start_col_centered(int scr_cols, int n);
void format_apps_display_text(
    char msg[MAX_APPS_DISPLAY_LEN],
    GameContext *ctx
);
void suffix_format(char *dest, double n);
void comma_format(char *dest, unsigned long n);

void draw(GameContext *ctx);
void draw_store(int rows, int cols, GameContext *ctx);
void draw_opts(int rows, int cols);
void create_box(
    int y,
    int x,
    int len,
    int height, 
    char corner_tl,
    char corner_tr,
    char corner_bl,
    char corner_br,
    char horiz,
    char vert
);

#endif
