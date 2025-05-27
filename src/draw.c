#include "draw.h"
#include <ncurses.h>

int get_opts_start_y(int rows) {
    // Opts line is always 7/8 down the screen
    return (rows * 7) / 8;
}

int get_store_header_start_y(int rows) {
    // Store header begins 1/8 down the screen
    return rows / 8;
}

int get_middle_x(int cols, int msg_len) {
    return (cols - msg_len) / 2;
}

/* Draws options line displayed on all screens */
void draw_opts(int rows, int cols, GameState state) {
    char *opts[MAX_OPTS];
    int count = 0;
    opts[count++] = "[q]: Quit";

    if (state == STATE_HOME) {
        opts[count++] = "[l]: Store";
    } else if (state == STATE_STORE) {
        opts[count++] = "[h]: Home";
        opts[count++] = "[j]: Down";
        opts[count++] = "[k]: Up";
    }

    // Concatenate all options with 10 spaces
    char opts_msg[MAX_MSG_CHARS] = {0};
    for (int i = 0; i < count; ++i) {
        strcat(opts_msg, opts[i]);
        if (i != count-1) {
            strcat(opts_msg, "          ");
        }
    }

    mvprintw(get_opts_start_y(rows), get_middle_x(cols, strlen(opts_msg)), opts_msg);
}

/* Draws home screen */
void draw_home(int rows, int cols, long apps) {
    char apps_msg[MAX_MSG_CHARS];
    sprintf(apps_msg, "Applications: %lu", apps);
    mvprintw(rows / 2, get_middle_x(cols, strlen(apps_msg)), apps_msg);
}

/* Draws store screen */
void draw_store(int rows, int cols, GameContext *ctx) {
    StoreContext *store = ctx->store;

    char msg[MAX_MSG_CHARS];
    sprintf(msg, "Applications: %lu", ctx->apps);
    mvprintw(get_store_header_start_y(rows), get_middle_x(cols, strlen("MARKETPLACE")), "MARKETPLACE");
    mvprintw(get_store_header_start_y(rows) + 1, get_middle_x(cols, strlen(msg)), msg);

    int items_start_y = get_store_header_start_y(rows) + 4;
    int drawn = 0;
    for (int i = 0; i < MAX_STORE_ITEMS && drawn < store->unlocked_count; ++i) {
        StoreItem *item = &store->items[i];
        if (!item->unlocked) continue;

        if (store->selected_item == drawn) {
            mvprintw(get_opts_start_y(rows) - 4, (cols - strlen(item->desc)) / 2, "%s", item->desc);
            attron(A_STANDOUT);
        }
        mvprintw(items_start_y + i, (cols - 25) / 2, "%d. %s (%d) [%d]", 
                drawn+1, item->name, item->price, item->quant);

        attroff(A_STANDOUT);
        drawn++;
      }
    
    int store_box_y = get_store_header_start_y(rows) + 2;
    create_box(
        store_box_y,
        get_middle_x(cols, STORE_ITEMS_WIDTH),
        STORE_ITEMS_WIDTH,
        get_opts_start_y(rows) - store_box_y - STORE_DESC_HEIGHT - 4,
        '+',
        '+',
        '+',
        '+',
        '-',
        '|'
    );
    create_box(
        get_opts_start_y(rows) - STORE_DESC_HEIGHT - 2,
        get_middle_x(cols, STORE_DESC_WIDTH),
        STORE_DESC_WIDTH,
        STORE_DESC_HEIGHT,
        '[',
        ']',
        '[',
        ']',
        '=',
        ' '
    );
}

void draw(GameContext *ctx) {
    int rows, cols;
    getmaxyx(stdscr, rows, cols);
    switch (ctx->cur_state) {
        case STATE_HOME:
            draw_home(rows, cols, ctx->apps);
            break;
        case STATE_STORE:
            draw_store(rows, cols, ctx);
            break;
    }
    draw_opts(rows, cols, ctx->cur_state);
    wborder(stdscr, '|', '|', '-', '-', '+', '+', '+', '+');
}

/* 
 * Creates a box starting at position x, y of a given width and height.
 * Corner, horizontal and vertical characters must be specified. User must 
 * still call 'refresh()' to actually render the box.
 */
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
) {	
	mvaddch(y, x, corner_tl);
    mvaddch(y, x + len, corner_tr);
    mvaddch(y + height, x, corner_bl);
    mvaddch(y + height, x + len, corner_br);
    mvhline(y, x + 1, horiz, len - 1);
    mvhline(y + height, x + 1, horiz, len - 1);
    mvvline(y + 1, x, vert, height - 1);
    mvvline(y + 1, x + len, vert, height - 1);
}
