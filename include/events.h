#ifndef EVENTS_H
#define EVENTS_H

#include "game_state.h"

#define MAX_KEYS 256

typedef enum {
    KEY_INACTIVE,
    KEY_PRESS,
    KEY_HOLD,
} KeyState;

void get_keys(KeyState keys[MAX_KEYS]);
int update(GameContext *ctx, KeyState keys[MAX_KEYS]);
void update_store(GameContext *ctx, KeyState keys[MAX_KEYS]);
void tick_update(GameContext *ctx);

#endif
