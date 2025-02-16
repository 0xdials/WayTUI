#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "seatd.h"

void setup_seatd(const char *username) {
    printf("Setting up seatd for user: %s\n", username);

    if (setenv("XDG_SEAT", "seat0", 1) != 0) {
        perror("Failed to set XDG_SEAT");
    }

    if (setenv("XDG_SESSION_TYPE", "wayland", 1) != 0) {
        perror("Failed to set XDG_SESSION_TYPE");
    }

    if (setenv("XDG_SESSION_CLASS", "user", 1) != 0) {
        perror("Failed to set XDG_SESSION_CLASS");
    }

    // Start a seatd session (not strictly required but whatever)
    if (system("seatd-launch") != 0) {
        printf("Warning: seatd-launch failed. Ensure seatd is running!\n");
    }
}

