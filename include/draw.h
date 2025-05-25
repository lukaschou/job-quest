#ifndef DRAW_H
#define DRAW_H

#include "game_state.h"
#include <string.h>

void draw(GameContext *ctx);
void draw_store(int rows, int cols, GameContext *ctx);
void draw_opts(int rows, int cols, GameState state);

#endif
