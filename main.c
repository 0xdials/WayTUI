#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <security/pam_appl.h>
#include <ncurses.h>

// PAM conversation function to handle password input
static int pam_conv(int num_msg, const struct pam_message **msg, 
                    struct pam_response **resp, void *appdata_ptr) {
    char *password = (char *)appdata_ptr;
    struct pam_response *response = calloc(num_msg, sizeof(struct pam_response));

    for (int i = 0; i < num_msg; i++) {
        if (msg[i]->msg_style == PAM_PROMPT_ECHO_OFF) {
            response[i].resp = strdup(password);
        }
    }

    *resp = response;
    return PAM_SUCCESS;
}

// Authenticate user via PAM
int authenticate(const char *username, const char *password) {
    pam_handle_t *pam_handle = NULL;
    struct pam_conv pam_conversation = { .conv = pam_conv, .appdata_ptr = (void *)password };

    int ret = pam_start("my-dm", username, &pam_conversation, &pam_handle);
    if (ret != PAM_SUCCESS) return 0;

    ret = pam_authenticate(pam_handle, 0);  // Authenticate
    if (ret == PAM_SUCCESS) ret = pam_acct_mgmt(pam_handle, 0);  // Check account validity

    pam_end(pam_handle, ret);
    return ret == PAM_SUCCESS;
}

// Launch Hyprland with proper environment
void launch_hyprland(const char *username) {
    pid_t pid = fork();
    if (pid == 0) {
        // Set Wayland-specific environment variables
        setenv("XDG_SESSION_TYPE", "wayland", 1);
        setenv("XDG_CURRENT_DESKTOP", "Hyprland", 1);
        setenv("HOME", getenv("HOME"), 1);  // Preserve home directory

        // Launch Hyprland (replace with your Hyprland command if needed)
        execlp("Hyprland", "Hyprland", NULL);
        perror("Failed to launch Hyprland");
        _exit(EXIT_FAILURE);
    } else {
        waitpid(pid, NULL, 0);  // Wait for Hyprland to exit
    }
}

// TUI for username/password input
int main() {
    // Initialize ncurses
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    // Get username/password
    char username[32];
    char password[32];

    printw("My Hyprland DM\n\n");
    printw("Username: ");
    refresh();
    getnstr(username, sizeof(username) - 1);

    printw("Password: ");
    refresh();
    noecho();
    getnstr(password, sizeof(password) - 1);
    echo();

    endwin();  // End ncurses

    // Authenticate and launch
    if (authenticate(username, password)) {
        printf("Authentication successful! Launching Hyprland...\n");
        launch_hyprland(username);
    } else {
        printf("Authentication failed.\n");
    }

    return 0;
}
