#include "system.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

/* Prints an error message and exits with a failure code */
void die(char *msg) {
    fprintf(stderr, "%s", msg);
    exit(EXIT_FAILURE);
}

/* Initialize ncurses subsystem */
void init_game() {
    if (!initscr()) {
        die("Failed to initialize ncurses screen\n");
    }
    // Set cursor to invisible
    if (curs_set(0) == ERR) {
        die("Terminal does not support invisible cursor\n");
    }
    if (raw() == ERR) {
        die("Failed to enter raw mode\n");
    }
    // Reads block for 100 milliseconds before returning ERR
    timeout(100); 
    if (keypad(stdscr, 1)) {
        die("Failed to enable terminal keypad\n");
    }
    if (noecho() == ERR) {
        die("Failed to disable echo\n");
    }
}

/* Destroy ncurses subsystem */
void cleanup() {
    if (endwin() == ERR) {
        die("Failed to clean up ncurses\n");
    }
}
