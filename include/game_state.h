#ifndef GAME_STATE_H
#define GAME_STATE_H

#define MAX_STORE_ITEMS 4
#define MAX_ITEM_QUANTITY 999

typedef struct {
  char *name;
  char desc[50];
  unsigned long price;
  int unlocked;
  int quant;
  int max_quant;
  float APS_bonus;
  float APS_unlock_rqmt;
  float price_inc;
} StoreItem;

typedef struct {
  int selected_item; 
  int unlocked_count;
  StoreItem items[MAX_STORE_ITEMS];
} StoreContext;

typedef enum {
    STATE_STORE,
} GameState;

typedef struct {
    GameState cur_state;
    StoreContext *store;
    double apps;
    double apps_per_sec;
} GameContext;

#endif
