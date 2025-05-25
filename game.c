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
  GameContext ctx;
  StoreContext store;
  KeyState keys[MAX_KEYS] = { KEY_INACTIVE };
  init_game(&ctx, &store);
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
