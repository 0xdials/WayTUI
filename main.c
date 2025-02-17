#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <security/pam_appl.h>
#include <ncurses.h>

// PAM conversation function
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

    int ret = pam_start("waytui", username, &pam_conversation, &pam_handle); // <-- Updated service name
    if (ret != PAM_SUCCESS) return 0;

    ret = pam_authenticate(pam_handle, 0);
    if (ret == PAM_SUCCESS) ret = pam_acct_mgmt(pam_handle, 0);

    pam_end(pam_handle, ret);
    return ret == PAM_SUCCESS;
}

// Launch Hyprland 
void launch_hyprland(const char *username) {
    pid_t pid = fork();
    if (pid == 0) {
        setenv("XDG_SESSION_TYPE", "wayland", 1);
        setenv("XDG_CURRENT_DESKTOP", "Hyprland", 1);
        setenv("HOME", getenv("HOME"), 1);

        execlp("Hyprland", "Hyprland", NULL);
        perror("Failed to launch Hyprland");
        _exit(EXIT_FAILURE);
    } else {
        waitpid(pid, NULL, 0);
    }
}

// TUI 
int main() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    char username[32];
    char password[32];

    printw("WayTUI - Hyprland Login\n\n"); 
    printw("Username: ");
    refresh();
    getnstr(username, sizeof(username) - 1);

    printw("Password: ");
    refresh();
    noecho();
    getnstr(password, sizeof(password) - 1);
    echo();

    endwin();

    if (authenticate(username, password)) {
        printf("Authenticated. Launching Hyprland...\n");
        launch_hyprland(username);
    } else {
        printf("Authentication failed.\n");
    }

    return 0;
}
