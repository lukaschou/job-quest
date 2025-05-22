#ifndef EVENTS_H
#define EVENTS_H

#define MAX_KEYS 256

typedef enum {
    KEY_INACTIVE,
    KEY_PRESS,
    KEY_HOLD,
} KeyState;

void handle_key_presses(KeyState keys[MAX_KEYS]);

#endif
