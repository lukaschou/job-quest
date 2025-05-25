#include "draw.h"
#include <ncurses.h>

void draw_opts(int rows, int cols, GameState state) {
    char *opts;
    if (state == STATE_HOME) {
        opts = "[q]: Quit               [s]: Store";
    } else if (state == STATE_STORE) {
        opts = "[q]: Quit               [m]: Main";
    }

    int opts_y = (rows * 7) / 8;
    int opts_x = (cols - strlen(opts)) / 2;
    mvprintw(opts_y, opts_x, opts);
}

void draw_main(int rows, int cols, long apps) {
    char apps_msg[50];
    sprintf(apps_msg, "Applications: %lu", apps);
    mvprintw(rows / 2, (cols - strlen(apps_msg)) / 2, apps_msg);
}

// This will need lots of reworking as the item list grows...
void draw_store(int rows, int cols, GameContext *ctx) {
    StoreContext *store = ctx->store;
    int top = (rows / 8) + 2;
    int bottom = (rows * 7) / 8;
    int box_height = bottom - top - 1;
    int header_y = rows / 8;
    int header_x = (cols - strlen("MARKETPLACE")) / 2;
    mvprintw(header_y, header_x, "MARKETPLACE");

    char msg[100];
    sprintf(msg, "Applications: %lu", ctx->apps);
    mvprintw(header_y + 1, (cols - strlen(msg)) / 2, msg);

    int start_y = (rows / 8) + 4;
    int drawn = 0;
    for (int i = 0; i < MAX_STORE_ITEMS && drawn < store->unlocked_count; ++i) {
        StoreItem *item = &store->items[i];
        if (!item->unlocked) continue;

        if (store->selected_item == drawn) {
            mvprintw(bottom - 3, (cols - strlen(item->desc)) / 2, "%s", item->desc);
            attron(A_STANDOUT);
        }
        mvprintw(start_y + i, (cols - 25) / 2, "%d. %s (%d) [%d]", 
                drawn+1, item->name, item->price, item->quant);

        attroff(A_STANDOUT);
        drawn++;
      }
    create_box(top, (cols - 50) / 2, 50, box_height - 5);
    create_box(bottom - 5, (cols - 70) / 2, 70, 4);
}

void draw(GameContext *ctx) {
    int rows, cols;
    getmaxyx(stdscr, rows, cols);
    switch (ctx->cur_state) {
        case STATE_HOME:
            draw_main(rows, cols, ctx->apps);
            break;
        case STATE_STORE:
            draw_store(rows, cols, ctx);
            break;
    }
    draw_opts(rows, cols, ctx->cur_state);
    wborder(stdscr, '|', '|', '-', '-', '+', '+', '+', '+');
}

void create_box(int y, int x, int len, int height) {	
	mvaddch(y, x, '+');
  mvaddch(y, x + len, '+');
  mvaddch(y + height, x, '+');
  mvaddch(y + height, x + len, '+');
  mvhline(y, x + 1, '=', len - 1);
  mvhline(y + height, x + 1, '=', len - 1);
  mvvline(y + 1, x, ' ', height - 1);
  mvvline(y + 1, x + len, ' ', height - 1);
}
