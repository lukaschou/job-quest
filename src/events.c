#include "events.h"
#include <ncurses.h>

void handle_key_presses(KeyState keys[MAX_KEYS]) {
    int ch = getch();
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
