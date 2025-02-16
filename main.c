#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "seatd.h"
#include "tui.h"

void setup_xdg_runtime() {
    char *xdg_runtime = getenv("XDG_RUNTIME_DIR");
    if (!xdg_runtime) {
        char path[256];
        snprintf(path, sizeof(path), "/run/user/%d", getuid());
        setenv("XDG_RUNTIME_DIR", path, 1);
        printf("XDG_RUNTIME_DIR set to: %s\n", path);
    }
}

int main() {
    char username[32];

    // Set up XDG_RUNTIME_DIR properly
    setup_xdg_runtime();

    // Start the TUI interface
    start_tui(username, sizeof(username));

    // Setup seatd (manages sessions)
    setup_seatd(username);

    // Start user session (launch Wayland compositor)
    start_session(username);

    return 0;
}

