#include <ncurses.h>
#include <string.h>
#include <unistd.h>

int main() {
  int row, col;
  int count = 0;
  initscr();
  curs_set(0);
  raw();
  timeout(1000);
  keypad(stdscr, 1);
  noecho();
  while (1) {
    clear();
    getmaxyx(stdscr, row, col);
    char msg[10];
    sprintf(msg, "Count: %d", count);
    mvprintw(row/2, (col-strlen(msg))/2, msg);
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