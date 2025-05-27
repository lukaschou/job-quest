#include "system.h"
#include <curses.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

/* Prints an error message and exits with a failure code */
void die_curses(char *msg) {
    fprintf(stderr, "%s", msg);
    exit(EXIT_FAILURE);
}

/* Calls perror with a message and exits with a failure code */
void die_sys(char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

/* Initializes and populates the store */
void init_store(StoreContext *store) {
  store->selected_item = 0;
  store->unlocked_count = 0;
  store->items[store->unlocked_count++] = (StoreItem){
    .name = "LeetCode",
    .desc = "Solve a medium. Cry. Repeat. Character-building stuff.",
    .price = 50,
    .quant = 0,
    .max_quant = MAX_ITEM_QUANTITY,
    .unlocked = 1,
    .APS_bonus = 1,
  };
  store->items[store->unlocked_count++] = (StoreItem){
    .name = "AI Cover Letter",
    .desc = "Looks good. No one read it anyway.",
    .price = 100,
    .quant = 0,
    .max_quant = MAX_ITEM_QUANTITY,
    .unlocked = 1,
    .APS_bonus = 2,
  };
  store->items[store->unlocked_count++] = (StoreItem){
    .name = "Buzzword Pack",
    .desc = "Synergize your leverage to optimize alignment.",
    .price = 200,
    .quant = 0,
    .max_quant = MAX_ITEM_QUANTITY,
    .unlocked = 1,
    .APS_bonus = 3,
  };
}


/* Initializes game context and screen system */
void init_game(GameContext *ctx, StoreContext *store) {
  init_store(store);
  ctx->cur_state = STATE_HOME;
  ctx->apps = 0;
  ctx->store = store;
  init_main_scr();
}

/* Initialize ncurses subsystem */
void init_main_scr() {
    if (!initscr()) {
        die_curses("Failed to initialize ncurses screen\n");
    }
    // Set cursor to invisible
    if (curs_set(0) == ERR) {
        die_curses("Terminal does not support invisible cursor\n");
    }
    if (raw() == ERR) {
        die_curses("Failed to enter raw mode\n");
    }
    // Reads block for 100 milliseconds before returning ERR
    timeout(100);
    if (keypad(stdscr, 1)) {
        die_curses("Failed to enable terminal keypad\n");
    }
    if (noecho() == ERR) {
        die_curses("Failed to disable echo\n");
    }
}

/* Destroy ncurses subsystem */
void cleanup() {
    if (endwin() == ERR) {
        die_curses("Failed to clean up ncurses\n");
    }
}
