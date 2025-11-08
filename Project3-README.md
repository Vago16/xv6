Name: Evagelos Petropoulos
U-Number: U75564437

1.  Test Environment:
        Docker shell with Ubuntu, then downloaded Dev Containers on host computer's (Mac running MAC OS Sequoia 15.6.1)
        Visual Studio Code, which allowed to edit the code from with Visual Studio Code.
2. Group Members: 
    Spencer Conrad- Talked about how to go about lazy allocation in any folder, and agreed that prepending '/' is the simplest solution.
    Hanieh Ajami-
    Research:
        https://courses.cs.duke.edu/spring25/compsci310/lazy.html

        
3. Additional Info:
    I have a PDF with screenshots for parts 1 and 2 called "project3-screenshots-part1&2"
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
    PART 2:
        To get the commands and lazy page allocationto work in any folder, prepend "/" to the command while inside of a folder that is not the xv6-public directory.
        Example while in different folder:
            $ /ls
    Part 3: 
        Similar to part 2, prepend commands with "/" to get commands to work in any folder.
    Part 4:
        My screenshots are in the folder named "screenshots_project3".
        B.
            A pure lazy allocation only allocates one page at a time as needed by a process.  The locality aware allocation extends the pure lazy allocation by allocating 3 continguous pages in memory at once.
            To first create the differences in whether a lazy or locality aware implementation would be used, I inlcuded these flags in Makefile so when make qemu-nox would be compiled, there could be a choice between them.
            MAKEFILE
            '''
            ALLOCATOR ?= LAZY	#defaults to LAZY
            ifeq ($(ALLOCATOR),LOCALITY)
            CFLAGS += -DLOCALITY
            else ifeq ($(ALLOCATOR),LAZY)
            CFLAGS += -DLAZY
            else
            CFLAGS += -DLAZY
            endif
            '''
            Then, in trap.c the page fault handler file, I included the if/else statements depending on which one was selected, and what code to run if so.  This took place between lines 155-220 in my trap.c code.  I also took walkpgdir() and mappages from vim.  walkpgdir() is used to check the page directory entry and allocates page tables if there are none; it is used in the mappages() function which maps virtual memory to physical pages and is used for both of the allocations in part 3.  Both implementations will continue to allocate pages as needed for the process called unless there is no physical memory left, in which case the process is terminated.
        C. I have my print statements, as shown in the screenshots, and they show the difference in what happens with lazy and locality allocation.  I made it so it in the for loop for the 3 pages being allocated in locality per page fault, that there would be a separate print statement each time showing which new physical address would be taken.  Since the lazy allocation only allocate one page per fault, it does not have any additional print statements like locality allocation did.
    