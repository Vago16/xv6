#include "types.h"
#include "stat.h"
#include "user.h"
//test to check if syscall for ticks_running works

int
main(void)
{
    int pid = getpid();
    printf(1, "Starting ticks_running test...\n");

    int start = ticks_running(pid);
    printf(1, "Ticks before %d\n", start);

    // simple workload — run a loop to use CPU time
    for (int i = 0; i < 50000000; i++) {
        asm volatile(""); // prevent optimization
    }

    int end = ticks_running(pid);
    printf(1, "Ticks used: %d\n", end - start);
    exit();
}
