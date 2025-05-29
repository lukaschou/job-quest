#include "draw.h"
#include "game_state.h"
#include <ncurses.h>
#include <math.h>

/* Draws options line displayed on all screens */
void draw_opts(int rows, int cols) {
    mvprintw(
        opts_start_row(rows),
        text_start_col_centered(cols, sizeof("[q]: Quit")),
        "[q]: Quit"
    );
}

/* Comma seperates a number less than one-million */
void comma_format(char *dest, unsigned long n) {
    char reversed[10];
    int i = 0, j = 0;
    do {
        if (i > 0 && i % 3 == 0) {
            reversed[j++] = ',';
        }
        reversed[j++] = '0' + (n % 10);
        n /= 10;
        i++;
    } while (n > 0);

    for (int k = 0; k < j; ++k) {
        dest[k] = reversed[j - k - 1];
    }
    dest[j] = '\0';
}

/* Format numbers with a suffix. Behavior is unspecified for values
 * beyond the trillions. */
void suffix_format(char *dest, double n) {
    char suffixes[] = { '\0', 'k', 'm', 'b', 't' }; 
    double value = n;
    int suffix_i = 0;   

    while (value >= 1000.0 && suffix_i < 4) {
        value /= 1000.0;
        suffix_i++;
    }
    
    value = floor(value * 1000.0) / 1000.0;
    sprintf(dest, "%.3f%c", value, suffixes[suffix_i]);
}

/* Formats application count and APS into a UI-friendly string */
void format_apps_display_text(
    char msg[MAX_APPS_DISPLAY_LEN],
    GameContext *ctx
    
) {
    char num_str[MAX_SUFFIX_FORMAT_LEN];
    char aps_str[MAX_SUFFIX_FORMAT_LEN];

    if (ctx->apps < SUFFIX_THRESHOLD) {
        comma_format(num_str, (unsigned int) ctx->apps);
    } else {
        suffix_format(num_str, ctx->apps);
    }
    
    suffix_format(aps_str, ctx->apps_per_sec);
    snprintf(
        msg,
        MAX_APPS_DISPLAY_LEN,
        "%s applications (%s/s)", 
        num_str,
        aps_str
    );
}

/* Calculate header start row based on screen height */
int store_header_start_row(int scr_rows) {
    return (scr_rows < SMALL_SCR_HEIGHT_THRESH) ? scr_rows / 10 : scr_rows / 8;
}

/* Caclulate column start for a centered message of length n */
int text_start_col_centered(int scr_cols, int n) {
    return (scr_cols - n) / 2;
}

/* Calculates game options start row based on screen height */
int opts_start_row(int scr_rows) {
    return (scr_rows < SMALL_SCR_HEIGHT_THRESH) 
        ? (scr_rows * 9) / 10
        : (scr_rows * 7) / 8;
}

/* Calculates store box width based on screen width */
int store_items_box_width(int scr_cols) {
    const int des_width = scr_cols / 3;
    if (des_width < MIN_STORE_BOX_WIDTH) {
        return MIN_STORE_BOX_WIDTH;
    } else if (des_width > MAX_STORE_BOX_WIDTH) {
        return MAX_STORE_BOX_WIDTH;
    } else {
        return des_width;
    }
}

/* Draws store screen */
void draw_store(int rows, int cols, GameContext *ctx) {
    StoreContext *store = ctx->store;

    /* Store header */
    char apps_display_msg[MAX_APPS_DISPLAY_LEN];
    format_apps_display_text(apps_display_msg, ctx);
    mvprintw(
        store_header_start_row(rows),
        text_start_col_centered(cols, strlen("MARKETPLACE")),
        "MARKETPLACE"
    );
    mvprintw(
        store_header_start_row(rows) + 1,
        text_start_col_centered(cols, strlen(apps_display_msg)),
        apps_display_msg
    );
   
    /* Store items */
    int store_items_box_start_row = store_header_start_row(rows) + 2;
    int store_items_box_start_col = 
        text_start_col_centered(cols, store_items_box_width(cols));
    // 2 is for padding between opts, desc box, and store box 
    int store_items_box_height = 
        opts_start_row(rows) 
        - ITEM_DESC_BOX_HEIGHT 
        - 2 
        - store_items_box_start_row;
    int store_items_list_start_row = store_items_box_start_row + 1;
    int store_items_box_capacity = store_items_box_height - 2;
    int start = store->selected_item >= store_items_box_capacity
        ? store->selected_item - store_items_box_capacity + 1
        : 0; 
    int desc_start_row = opts_start_row(rows) - 5;
    int drawn = 0;

    for (
        int i = start;
        i < store->unlocked_count && drawn < store_items_box_capacity;
        ++i
    ) {
        StoreItem *item = &store->items[i];
        if (store->selected_item == i) {
            // Print description
            mvprintw(
                desc_start_row,
                text_start_col_centered(cols, strlen(item->desc)),
                "%s",
                item->desc
            );
            char item_stats[MIN_STORE_BOX_WIDTH];
            snprintf(
                item_stats,
                MIN_STORE_BOX_WIDTH,
                "Apps/sec: %.2f",
                item->APS_bonus
            );
            mvprintw(
                desc_start_row + 1,
                text_start_col_centered(cols, strlen(item_stats)),
                "%s",
                item_stats
            );
            attron(A_STANDOUT); // Highlight
        }
        // Print item name and price
        mvprintw(
            store_items_list_start_row + drawn,
            store_items_box_start_col + 2,
            "%s (%d)",
            item->name,
            item->price
        ); 
        attroff(A_STANDOUT);
        // Print quantity on right side (MAX 3 digits)
        mvprintw(
            store_items_list_start_row + drawn,
            store_items_box_start_col + store_items_box_width(cols) - 5,
            item->quant < item->max_quant ? "x%03d" : "xMAX",
            item->quant
        ); 
        drawn++;
    }

    /* Boxes */
    create_box(
        store_items_box_start_row,
        store_items_box_start_col,
        store_items_box_width(cols),
        store_items_box_height,
        '+',
        '+',
        '+',
        '+',
        '-',
        '|'
    );
    create_box(
        opts_start_row(rows) - ITEM_DESC_BOX_HEIGHT - 1,
        text_start_col_centered(cols, store_items_box_width(cols)),
        store_items_box_width(cols),
        ITEM_DESC_BOX_HEIGHT,
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
        case STATE_STORE:
            draw_store(rows, cols, ctx);
            break;
    }
    draw_opts(rows, cols);
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
    mvaddch(y + height - 1, x, corner_bl);
    mvaddch(y + height - 1, x + len, corner_br);
    mvhline(y, x + 1, horiz, len - 1);
    mvhline(y + height - 1, x + 1, horiz, len - 1);
    mvvline(y + 1, x, vert, height - 2);
    mvvline(y + 1, x + len, vert, height - 2);
}
