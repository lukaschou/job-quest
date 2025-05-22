#ifndef GAME_STATE_H
#define GAME_STATE_H

typedef enum {
    STATE_MAIN,
    STATE_STORE,
} GameState;

typedef struct {
    GameState cur_state;
    long apps;
} GameContext;

#endif
