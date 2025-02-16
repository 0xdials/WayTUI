#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <string.h>
#include "seatd.h"
#include "tui.h"

#define SESSION_CMD "Hyprland" // You can replace this with your Wayland compositor if needed

// Function to start the Wayland session
void start_session(const char *username) {
    // Get the user info based on the username
    struct passwd *pw = getpwnam(username);
    if (!pw) {
        printf("Error: User not found.\n");
        exit(EXIT_FAILURE);
    }

    // Set user credentials (so we can run the session as the user)
    setgid(pw->pw_gid); // Set group ID
    setuid(pw->pw_uid); // Set user ID

    // Set up environment variables (not strictly necessary, but nice)
    setenv("XDG_SESSION_TYPE", "wayland", 1);
    setenv("XDG_SESSION_CLASS", "user", 1);
    setenv("XDG_SEAT", "seat0", 1);

    // Finally, execute the Wayland session (Hyprland)
    if (execlp(SESSION_CMD, SESSION_CMD, NULL) == -1) {
        perror("Failed to start Wayland session");
        exit(EXIT_FAILURE);
    }
}

int main() {
    char username[32];

    // Start our *fancy* TUI
    init_tui();

    // Draw login prompt
    draw_login_prompt(username);

    // Clean up TUI before switching sessions
    cleanup_tui();

    // Set up seatd for the user
    setup_seatd(username);

    // Start the session (We call the start_session() function now)
    start_session(username);

    return 0;
}

