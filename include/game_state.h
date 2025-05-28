#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <ncurses.h>

#define MAX_STORE_ITEMS 3
#define MAX_ITEM_QUANTITY 999

typedef struct {
  char *name;
  char *desc;
  unsigned long price;
  int unlocked;
  int quant;
  int max_quant;
  float APS_bonus;
  float price_inc;
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
    double apps;
    double apps_per_sec;
} GameContext;

#endif
