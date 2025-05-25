#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <ncurses.h>

#define MAX_STORE_ITEMS 3

typedef struct {
  char *name;
  char *desc;
  unsigned long price;
  int unlocked;
  int quant;
  int max_quant;
  unsigned long bonus_apps;
} StoreItem;

typedef struct {
  int selected_item; 
  int unlocked_count;
  StoreItem items[MAX_STORE_ITEMS];
} StoreContext;

typedef enum {
    STATE_HOME,
    STATE_STORE,
} GameState;

typedef struct {
    GameState cur_state;
    StoreContext *store;
    unsigned long apps;
    unsigned long apps_per_sec;
} GameContext;

#endif
