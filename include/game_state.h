#ifndef GAME_STATE_H
#define GAME_STATE_H

#define MAX_STORE_ITEMS 50

typedef enum {
    STATE_MAIN,
    STATE_STORE,
} GameState;

typedef struct {
    char *name;
    char *desc;
    int price;
    int visible;
    int quant;
    int max_quant;
} StoreItem;

typedef struct {
    int selected_item;
    int item_count;
    StoreItem items[MAX_STORE_ITEMS];
} StoreState;

typedef struct {
    GameState cur_state;
    long apps;
    StoreState *store;
} GameContext;

#endif
