#include "events.h"
#include "game_state.h"
#include "system.h"
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

void draw_clicker(int rows, int cols, int count) {
    char apps[24];
    sprintf(apps, "Applications: %09d", count);
    mvprintw(rows / 2, (cols - strlen(apps)) / 2, apps);
}

void draw_store(int rows, int cols, int count, KeyState keys[MAX_KEYS], int *highlight) {
    mvprintw(rows / 8, (cols - 11) / 2, "MARKETPLACE");
    char msg[100];
    sprintf(msg, "Applications: %d", count);
    mvprintw((rows / 8) + 1, (cols - strlen(msg)) / 2, msg);
    char *items[] = {
        "Leetcode practice",
        "Udemy courses",
        "AWS certification",
    };

    if (keys['k'] && *highlight > 0) {
        (*highlight)--;
    } else if (keys['j'] && *highlight < 2) {
        (*highlight)++;
    }
    int start_y = (rows / 8) + 3;
    for (int i = 0; i < 3; ++i) {
        if (*highlight == i) {
            attron(A_STANDOUT);
        }
        mvprintw(start_y + i, (cols - 25) / 2, "%d. %s", i + 1, items[i]);
        attroff(A_STANDOUT);
    }
}

void draw_opts(int rows, int cols, int count, GameState state) {
    char *opts;
    if (state == STATE_MAIN) {
        opts = "[q]: Quit               [s]: Store";
    } else if (state == STATE_STORE) {
        opts = "[q]: Quit               [m]: Main";
    }
    mvprintw((rows * 4) / 5, (cols - strlen(opts)) / 2, opts);
}

int main() {
  GameContext ctx = { .cur_state = STATE_MAIN, .apps = 0 };
  KeyState keys[MAX_KEYS] = { KEY_INACTIVE };

  int rows, cols;
  int highlight = 0;
  init_game();
  while (1) {
    clear();
    getmaxyx(stdscr, rows, cols);
    switch(ctx.cur_state) {
        case STATE_MAIN:
            draw_clicker(rows, cols, ctx.apps);
            break;
        case STATE_STORE:
            draw_store(rows, cols, ctx.apps, keys, &highlight);
            break;
    }
    draw_opts(rows, cols, ctx.apps, ctx.cur_state);
    wborder(stdscr, '|', '|', '-', '-', '+', '+', '+', '+');
   
    get_keys(keys);

    if (keys[' '] == KEY_PRESS) {
        ctx.apps++;
    } else if (keys['q']) {
        break;
    } else if (keys['s'] && ctx.cur_state == STATE_MAIN) {
        highlight = 0;
        ctx.cur_state = STATE_STORE;
    } else if (keys['m'] && ctx.cur_state == STATE_STORE) {
        ctx.cur_state = STATE_MAIN;
    } 
  } 
  refresh();
  cleanup();
  return 0;
}
