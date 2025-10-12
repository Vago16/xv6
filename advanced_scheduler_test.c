#include "types.h"
#include "stat.h"
#include "user.h"

int main(void) {
    printf(1, "Starting priority scheduler test...\n");

    int p1 = fork();
    if (p1 == 0) {
        set_sched_priority(0); // High priority
        for (int i = 0; i < 10; i++) {
            printf(1, "[P0] High priority running %d\n", i);
            sleep(1);
        }
        exit();
    }

    int p2 = fork();
    if (p2 == 0) {
        set_sched_priority(1); // Medium priority
        for (int i = 0; i < 10; i++) {
            printf(1, "[P1] Medium priority running %d\n", i);
            sleep(1);
        }
        exit();
    }

    int p3 = fork();
    if (p3 == 0) {
        set_sched_priority(2); // Low priority
        for (int i = 0; i < 10; i++) {
            printf(1, "[P2] Low priority running %d\n", i);
            sleep(1);
        }
        exit();
    }

    sleep(1);
    // Get priorities before waiting (while procs still exist) so -1 is not returned
    int pr1 = get_sched_priority(p1);
    int pr2 = get_sched_priority(p2);
    int pr3 = get_sched_priority(p3);

    wait();
    wait();
    wait();

    printf(1, "\nPriority test complete.\n");
    printf(1, "PID %d priority: %d\n", p1, pr1);
    printf(1, "PID %d priority: %d\n", p2, pr2);
    printf(1, "PID %d priority: %d\n", p3, pr3);

    exit();
}
