#include "types.h"
#include "stat.h"
#include "user.h"

int main(void) {
    char *cmds[][2] = {
        {"hello", 0},
        {"ls", 0},
        {"uniq", 0},
        {"find", 0},
    };

    int n = sizeof(cmds)/sizeof(cmds[0]);
    int pids[4];
    int start_ticks[4];
    int end_ticks[4];
    int i;

    // Launch all processes
    for (i = 0; i < n; i++) {
        int pid = fork();
        if (pid == 0) {
            // Child runs command
            exec(cmds[i][0], cmds[i]);
            printf(1, "exec %s failed\n", cmds[i][0]);
            exit();
        } else if (pid > 0) {
            pids[i] = pid;
            start_ticks[i] = ticks_running(pid);
        }
    }

    // Wait for all and record completion
    for (i = 0; i < n; i++) {
        int pid = wait();
        end_ticks[i] = ticks_running(pids[i]);
    }
    exit();
}
