#include "draw.h"
#include <ncurses.h>

void draw_opts(int rows, int cols, GameState state) {
    char *opts;
    if (state == STATE_HOME) {
        opts = "[q]: Quit               [s]: Store";
    } else if (state == STATE_STORE) {
        opts = "[q]: Quit               [m]: Main";
    }
    mvprintw((rows * 4) / 5, (cols - strlen(opts)) / 2, opts);
}

void draw_main(int rows, int cols, long apps) {
    char apps_msg[50];
    sprintf(apps_msg, "Applications: %lu", apps);
    mvprintw(rows / 2, (cols - strlen(apps_msg)) / 2, apps_msg);
}

// This will need lots of reworking as the item list grows...
void draw_store(int rows, int cols, GameContext *ctx) {
    StoreContext *store = ctx->store;
    mvprintw(rows / 8, (cols - 11) / 2, "MARKETPLACE");
   
    char msg[100];
    sprintf(msg, "Applications: %lu", ctx->apps);
    mvprintw((rows / 8) + 1, (cols - strlen(msg)) / 2, msg); 
    
    int start_y = (rows / 8) + 3;
    int drawn = 0;
    for (int i = 0; i < MAX_STORE_ITEMS && drawn < store->unlocked_count; ++i) {
        StoreItem *item = &store->items[i];
        if (!item->unlocked) continue;

        if (store->selected_item == drawn) {
            mvprintw((rows * 3) / 5, (cols - 25) / 2, "%s", item->desc);
            attron(A_STANDOUT);
        }
        mvprintw(start_y + i, (cols - 25) / 2, "%d. %s (%d) [%d]", 
                drawn+1, item->name, item->price, item->quant);

        attroff(A_STANDOUT);
        drawn++;
    }
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
