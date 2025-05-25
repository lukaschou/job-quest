#ifndef GAME_STATE_H
#define GAME_STATE_H

typedef enum {
    STATE_MAIN,
    STATE_STORE,
} GameState;

typedef struct {
    int selected_item;
} StoreState;

typedef struct {
    GameState cur_state;
    long apps;
    StoreState *store;
} GameContext;

#endif
