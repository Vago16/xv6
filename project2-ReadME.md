Name: Evagelos Petropoulos
U-Number: U75564437

1.  Test Environment:
        Docker shell with Ubuntu, then downloaded Dev Containers on host computer's (Mac running MAC OS Sequoia 15.6.1)
        Visual Studio Code, which allowed to edit the code from with Visual Studio Code.
2. Group Members: same as before, no specific info was really traded
    Spencer Conrad-
    Hanieh Ajami-
    Research:
         https://www.reddit.com/r/C_Programming/comments/icpuvn/function_to_convert_uppercase_letters_to_lowercase/

         https://github.com/marf/xv6-scheduling -- help with understanding implementing schedulers in xv6
3. Additional Info:
    PART 2:
    For the test for part 2, ticks_running() function, I added a test file called ticks_running_test, to run it in qemu-nox, run the command:
    $ ticks_running_

    PART 3:
    For the test for part 3, the test file is simple_scheduler_test.c(which shows the commands being run in the order of shortest to longest).  First you must run:
    $ make clean
    then run this to change the scheduler to shortest job first:
    $ make qemu-nox SCHEDULER=SJF
    then to run the test in qemu-nox, run the command:
    $ simple_schedul

    PART 4:
    In part 4, allocproc() in proc.c holds the quantum time, which I set as a default to 5, but can be changed.  For whatever reason, the default priority, 1, is being applied to both the latter processes in the test so the output is 0, 1, 1 instead of 0, 1, 2 as it should be.  To run advanced_scheduler_test command, first run:
    $ make clean
    then run this to change the scheduler to the multi level priority with round-robin:
    $ make qemu-nox SCHEDULER=PRIORITYRR

4. Report of Comparison:
    There were three schedulers that I encountered in this assignment.  The default scheduler in xv6 is round-robin.  For part 3, I had to implement a Shortest Job First scheduler and for part 4, a multi level priority scheduler with round-robin if two processes share the same priority.
    The default scheduler is simple and predictable, thus requires less code and computational overhead, and low risk of jobs starving. However, it cannot predict job length or prioritize important tasks so there can be a long wait for important jobs.
    The SJF scheduler works well with reducing turnaround time, esppecially for the shorter jobs that may have been waiting while a longer job would have been taking place.  However, since the longer jobs have to wait, they may starve.
    The multi level priority scheduler with round robin scheduler is great for assigning priority to important tasks and letting those finish before lesser tasks.  However, there is greater computation overhead with the quantum having to be computed and lower priority tasks can starve if higher priority tasks do not finish and give up the resources needed to run.