#include <curses.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include <unistd.h>

void die(char *msg) {
    fprintf(stderr, msg);
    exit(EXIT_FAILURE);
}

void init_game() {
    if (!initscr()) {
        die("Failed to initialize ncurses screen");
    }
    // Set cursor to invisible
    if (curs_set(0) == ERR) {
        die("Terminal does not support invisible cursor");
    }
    if (raw() == ERR) {
        die("Failed to enter raw mode");
    }
    // Reads block for 100 milliseconds before returning ERR
    timeout(100); 
    if (keypad(stdscr, 1)) {
        die("Failed to enable terminal keypad");
    }
    if (noecho() == ERR) {
        die("Failed to disable echo");
    }
}

int main() {
  int row, col;
  int count = 0;
  init_game();

  while (1) {
    clear();
    getmaxyx(stdscr, row, col);
    char msg[24];
    sprintf(msg, "Applications: %09d", count);
    mvprintw(row/2, (col-strlen(msg))/2, msg);
    char *m = "[q]: Quit               [s]: Store";
    mvprintw((row * 4)/5, (col-strlen(m))/2, m);
    wborder(stdscr, '|', '|', '-', '-', '+', '+', '+', '+');
    refresh();

    int ch = getch();
    if (ch == ' ') {
      count++;
    } else if (ch == 'q') {
      break;
    }
  }

  endwin();
  return 0;
}
