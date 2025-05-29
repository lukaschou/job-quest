#include "events.h"
#include "game_state.h"
#include <ncurses.h>
#include <math.h>

/* Update key state */
void get_keys(KeyState keys[MAX_KEYS]) {
    int ch = getch();
    // Ignore non-ASCII chars
    if (ch >= 0 && ch <= MAX_KEYS) {
        if (keys[ch] == KEY_INACTIVE) {
            keys[ch] = KEY_PRESS;
        } else if (keys[ch] == KEY_PRESS) {
            keys[ch] = KEY_HOLD;
        }
    } else if (ch == ERR) {
        for (int i = 0; i < MAX_KEYS; ++i) {
            keys[i] = KEY_INACTIVE;
        }
    }
}

/* 
 * Updates game context given current key state, returns 1 on update and 
 * 0 on exit request
 */
int update(GameContext *ctx, KeyState keys[MAX_KEYS]) {
    if (keys['q']) {
        return 0;
    } else if (keys[' '] == KEY_PRESS) {
        ctx->apps++;
    } else if (ctx->cur_state == STATE_STORE) {
        update_store(ctx, keys);
    }

    return 1;
}

void update_store(GameContext *ctx, KeyState keys[MAX_KEYS]) {
    StoreContext *store = ctx->store;

    if (keys['k'] && store->selected_item > 0) {
        store->selected_item--;
    } else if (keys['j'] && store->selected_item < store->unlocked_count - 1) {
        store->selected_item++;
    } else if (keys['b']) { // buy item
        StoreItem* selectedItem = &store->items[store->selected_item];
        if (selectedItem->price <= ctx->apps && selectedItem->quant < selectedItem->max_quant) 
        {
            selectedItem->quant += 1;
            ctx->apps -= selectedItem->price;
            ctx->apps_per_sec += selectedItem->APS_bonus;
            selectedItem->price = ceil(selectedItem->price * selectedItem->price_inc);
        }
    }
}

void tick_update(GameContext *ctx) {
    ctx->apps += ctx->apps_per_sec;
} 
