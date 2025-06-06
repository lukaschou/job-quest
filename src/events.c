#include "events.h"
#include "game_state.h"
#include "system.h"
#include <ncurses.h>
#include <math.h>
#include <time.h>

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

long get_time() {
    struct timespec time;
    if (clock_gettime(CLOCK_MONOTONIC, &time) == -1) {
        die_sys("clock_gettime");
    }

    return time.tv_sec * 1000000 + time.tv_nsec / 1000;
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
        if (
            selectedItem->price <= ctx->apps &&
            selectedItem->quant < selectedItem->max_quant
        ) {
            selectedItem->quant += 1;
            ctx->apps -= selectedItem->price;
            ctx->apps_per_sec += selectedItem->APS_bonus;
            selectedItem->price = ceil(selectedItem->price * selectedItem->price_inc);
            StoreItem *next_item = &store->items[store->unlocked_count];
            if (
                store->unlocked_count < MAX_STORE_ITEMS &&
                ctx->apps_per_sec >= next_item->APS_unlock_rqmt
            ) {
                next_item->unlocked = 1;
                store->unlocked_count++; 
            }
        }
    }
}

void tick_update(GameContext *ctx, double d_time) {
    double seconds = d_time / 1000000;
    ctx->apps += ctx->apps_per_sec * seconds;
} 
