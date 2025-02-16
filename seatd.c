#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void setup_seatd(const char *username) {
    printf("Setting up seatd for user: %s\n", username);

    pid_t pid = fork();
    if (pid == 0) { // Child process
        execlp("seatd", "seatd", "-g", "video", "--seat", "seat0", NULL);
        perror("Failed to start seatd");
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        perror("Fork failed for seatd");
        exit(EXIT_FAILURE);
    }
}

void start_session(const char *username) {
    printf("Starting session for user: %s\n", username);

    // Set XDG_SESSION_TYPE to wayland
    setenv("XDG_SESSION_TYPE", "wayland", 1);

    // Switch to user
    if (setuid(getpwnam(username)->pw_uid) == -1) {
        perror("Failed to switch user");
        exit(EXIT_FAILURE);
    }

    // Launch Hyprland
    execlp("Hyprland", "Hyprland", NULL);

    perror("Failed to start Hyprland");
    exit(EXIT_FAILURE);
}

