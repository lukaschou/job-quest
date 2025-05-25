#ifndef SYSTEM_H
#define SYSTEM_H

#include "game_state.h"

void die_curses(char *msg);
void die_sys(char *msg);
void init_main_scr();
void init_store(StoreContext *store);
void init_game(GameContext *ctx, StoreContext *store);
void cleanup();

#endif
