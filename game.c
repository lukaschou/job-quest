#include "system.h"
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include <sys/_types/_timespec.h>
#include <unistd.h>
#include <time.h>

#define MAX_CHARS 256

typedef enum {
    STATE_MAIN,
    STATE_STORE,
} GameState;

typedef struct {
    GameState cur_state;
} GameContext;

void draw_clicker(int rows, int cols, int count) {
    char apps[24];
    sprintf(apps, "Applications: %09d", count);
    mvprintw(rows / 2, (cols - strlen(apps)) / 2, apps);
}

void draw_store(int rows, int cols, int count, int key_presses[MAX_CHARS], int *highlight) {
    mvprintw(rows / 8, (cols - 11) / 2, "MARKETPLACE");
    char msg[100];
    sprintf(msg, "Applications: %d", count);
    mvprintw((rows / 8) + 1, (cols - strlen(msg)) / 2, msg);
    char *items[] = {
        "Leetcode practice",
        "Udemy courses",
        "AWS certification",
    };

    if (key_presses['k'] && *highlight > 0) {
        (*highlight)--;
    } else if (key_presses['j'] && *highlight < 2) {
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
  GameContext ctx = { .cur_state = STATE_MAIN };
  int key_presses[MAX_CHARS];
  int rows, cols;
  int count = 0;
  int highlight = 0;
  init_game();
  while (1) {
    clear();
    getmaxyx(stdscr, rows, cols);
    switch(ctx.cur_state) {
        case STATE_MAIN:
            draw_clicker(rows, cols, count);
            break;
        case STATE_STORE:
            draw_store(rows, cols, count, key_presses, &highlight);
            break;
    }
    draw_opts(rows, cols, count, ctx.cur_state);
    wborder(stdscr, '|', '|', '-', '-', '+', '+', '+', '+');
    refresh();
    for (int i = 0; i < MAX_CHARS; ++i) {
        key_presses[i] = 0;
    }

    int ch = getch();
    if (ch >= 0 && ch < MAX_CHARS) {
        key_presses[ch] = 1;
    }

    if (key_presses[' '] && ctx.cur_state == STATE_MAIN) {
      count++;
    } else if (key_presses['q']) {
      break;
    } else if (key_presses['s'] && ctx.cur_state == STATE_MAIN) {
        highlight = 0;
        ctx.cur_state = STATE_STORE;
    } else if (key_presses['m'] && ctx.cur_state == STATE_STORE) {
        ctx.cur_state = STATE_MAIN;
    }
  } 
  cleanup();
  return 0;
}
