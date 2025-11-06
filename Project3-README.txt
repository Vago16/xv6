Name: Evagelos Petropoulos
U-Number: U75564437

1.  Test Environment:
        Docker shell with Ubuntu, then downloaded Dev Containers on host computer's (Mac running MAC OS Sequoia 15.6.1)
        Visual Studio Code, which allowed to edit the code from with Visual Studio Code.
2. Group Members: same as before, no specific info was really traded
    Spencer Conrad-
    Hanieh Ajami-
    Research:
        https://courses.cs.duke.edu/spring25/compsci310/lazy.html

        
3. Additional Info:
    PART 1: 
        Page allocation in xv6 is implemented by a program calling sbrk(), and sbrk() requests a page to allocate.
    n in argint(0, &n) stands for the number of bytes that the system call from the user asks for the page size.
    addr variable saves the current memory before growing, then records the amount of virtual memory allocated for the process.
    The call to growprc() allocates physical memory pages for the process.
        What breaks in xv6 is any user program that tries to write more than its original allocation of memory.
    The reason whu it  will crash upon calling a process is that when it tries to allocate or write memory beyond
    its original bounds, there is no addtional new physical pages that can be allocated for the virtual memory.  The memory 
    management unit is not aware of the kernel trying to access more memory address, so they end up pointing to nothing.
        The "trap" error is referring to the page faults when it tries to access invalid addresses.  The virtual memory is not being
    correctly translated to physical memory without the call to growprc(), this is what "0x1204 address" refers to.  Thus, xv6 kills the process.
        Without sbrk(), no process could grow beyond its initial bounds and thus xv6 would be very limited in what it could do.
