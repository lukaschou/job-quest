#include "draw.h"
#include <ncurses.h>

void draw_opts(int rows, int cols, GameState state) {
    char *opts;
    if (state == STATE_MAIN) {
        opts = "[q]: Quit               [s]: Store";
    } else if (state == STATE_STORE) {
        opts = "[q]: Quit               [m]: Main";
    }
    mvprintw((rows * 4) / 5, (cols - strlen(opts)) / 2, opts);
}

void draw_main(int rows, int cols, long apps) {
    char apps_msg[24];
    sprintf(apps_msg, "Applications: %09lu", apps);
    mvprintw(rows / 2, (cols - strlen(apps_msg)) / 2, apps_msg);
}

void draw_store(int rows, int cols, long count, StoreState *store) {
    mvprintw(rows / 8, (cols - 11) / 2, "MARKETPLACE");
    char msg[100];
    sprintf(msg, "Applications: %lu", count);
    mvprintw((rows / 8) + 1, (cols - strlen(msg)) / 2, msg);
    char *items[] = {
        "Leetcode practice",
        "Udemy courses",
        "AWS certification",
    };

    int start_y = (rows / 8) + 3;
    for (int i = 0; i < 3; ++i) {
        if (store->selected_item == i) {
            attron(A_STANDOUT);
        }
        mvprintw(start_y + i, (cols - 25) / 2, "%d. %s", i + 1, items[i]);
        attroff(A_STANDOUT);
    }
}

void draw(GameContext *ctx) {
    int rows, cols;
    getmaxyx(stdscr, rows, cols);
    switch (ctx->cur_state) {
        case STATE_MAIN:
            draw_main(rows, cols, ctx->apps);
            break;
        case STATE_STORE:
            draw_store(rows, cols, ctx->apps, ctx->store);
            break;
    }
    draw_opts(rows, cols, ctx->cur_state);
    wborder(stdscr, '|', '|', '-', '-', '+', '+', '+', '+');
}
