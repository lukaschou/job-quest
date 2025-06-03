#include "draw.h"
#include "events.h"
#include "game_state.h"
#include "system.h"
#include <ncurses.h>
#include <unistd.h>
#include <time.h>

int main() {
  GameContext ctx;
  StoreContext store;
  KeyState keys[MAX_KEYS] = { KEY_INACTIVE };
  init_game(&ctx, &store);

  long last_tick = get_time();
  while (1) {
    clear();
    draw(&ctx);
    get_keys(keys);
    
    long now = get_time();
    if (now > last_tick + 10000) {
        tick_update(&ctx, now - last_tick);
        last_tick = now;
    }
    // We don't need to 'refresh' since the getch() call in update does this
    if (!update(&ctx, keys)) {
        break;
    } 
  } 
  cleanup();
  return 0;
}
