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

  time_t last_tick = time(NULL);
  while (1) {
    clear();
    draw(&ctx);
    get_keys(keys);
    
    time_t now = time(NULL);
    if (now > last_tick) {
      last_tick = now;
      update_aps(&ctx);
    }
    // We don't need to 'refresh' since the getch() call in update does this
    if (!update(&ctx, keys)) {
        break;
    } 
  } 
  cleanup();
  return 0;
}
