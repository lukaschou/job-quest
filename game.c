#include "draw.h"
#include "events.h"
#include "game_state.h"
#include "system.h"
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

int main() {
  StoreItem test = { 
    .name = "Harvard CS50",
    .desc = "An introduction to Computer Science.",
    .price = 100,
    .visible = 1,
    .quant = 0,
    .max_quant = 1
  };
  StoreState store = { .selected_item = 0, .item_count = 1, .items[0] = test };
  GameContext ctx = { .cur_state = STATE_MAIN, .apps = 0, &store };
  KeyState keys[MAX_KEYS] = { KEY_INACTIVE };

  init_game();
  while (1) {
    clear();
    draw(&ctx);
    get_keys(keys);
    // We don't need to 'refresh' since the getch() call in update does this
    if (!update(&ctx, keys)) {
        break;
    } 
  } 
  cleanup();
  return 0;
}
