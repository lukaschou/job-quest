#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include <sys/_types/_timespec.h>
#include <unistd.h>
#include <time.h>

typedef enum {
    STATE_MAIN,
    STATE_STORE,
} GameState;

typedef struct {
    GameState cur_state;
} GameContext;

void die(char *msg) {
    fprintf(stderr, "%s", msg);
    exit(EXIT_FAILURE);
}

void init_game() {
    if (!initscr()) {
        die("Failed to initialize ncurses screen\n");
    }
    // Set cursor to invisible
    if (curs_set(0) == ERR) {
        die("Terminal does not support invisible cursor\n");
    }
    if (raw() == ERR) {
        die("Failed to enter raw mode\n");
    }
    // Reads block for 100 milliseconds before returning ERR
    timeout(100); 
    if (keypad(stdscr, 1)) {
        die("Failed to enable terminal keypad\n");
    }
    if (noecho() == ERR) {
        die("Failed to disable echo\n");
    }
}

void draw_clicker(int rows, int cols, int count) {
    char apps[24];
    sprintf(apps, "Applications: %09d", count);
    mvprintw(rows / 2, (cols - strlen(apps)) / 2, apps);
}

void draw_store(int rows, int cols, int count) {
    mvprintw(rows / 8, (cols - 11) / 2, "MARKETPLACE");
    char msg[100];
    sprintf(msg, "Applications: %d", count);
    mvprintw((rows / 8) + 1, (cols - strlen(msg)) / 2, msg);
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
  int rows, cols;
  int count = 0;
  int ticks = 0;
  init_game();
  struct timespec last_time, cur_time;
  clock_gettime(CLOCK_MONOTONIC, &last_time);

  while (1) {
    clear();
    getmaxyx(stdscr, rows, cols);
    switch(ctx.cur_state) {
        case STATE_MAIN:
            draw_clicker(rows, cols, count);
            break;
        case STATE_STORE:
            draw_store(rows, cols, count);
            break;
    }
    draw_opts(rows, cols, count, ctx.cur_state);
    wborder(stdscr, '|', '|', '-', '-', '+', '+', '+', '+');
    refresh();

    int ch = getch();
    if (ch == ' ' && ctx.cur_state == STATE_MAIN) {
      count++;
    } else if (ch == 'q') {
      break;
    } else if (ch == 's' && ctx.cur_state == STATE_MAIN) {
        ctx.cur_state = STATE_STORE;
    } else if (ch == 'm' && ctx.cur_state == STATE_STORE) {
        ctx.cur_state = STATE_MAIN;
    }
    // Auto-increment every second
    clock_gettime(CLOCK_MONOTONIC, &cur_time);
    if ((cur_time.tv_sec - last_time.tv_sec) >= 1) {
        int seconds_passed = cur_time.tv_sec - last_time.tv_sec;
        count += seconds_passed;
        last_time = cur_time;
    }
    ticks++;
  }

  if (endwin() == ERR) {
      die("Failed to clean up ncurses stuff\n");
  }
  return 0;
}
