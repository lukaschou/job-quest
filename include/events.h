#ifndef EVENTS_H
#define EVENTS_H

#define MAX_KEYS 256

typedef enum {
    KEY_INACTIVE,
    KEY_PRESS,
    KEY_HOLD,
} KeyState;

void get_keys(KeyState keys[MAX_KEYS]);

#endif
