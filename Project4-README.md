Name: Evagelos Petropoulos
U-Number: U75564437

1.  Test Environment:
        Docker shell with Ubuntu, then downloaded Dev Containers on host computer's (Mac running MAC OS Sequoia 15.6.1)
        Visual Studio Code, which allowed to edit the code from with Visual Studio Code.
2. Group Members: 
    Spencer Conrad- 
    Hanieh Ajami-
    Research:
        https://web.cs.ucla.edu/classes/winter12cs111/scribe/11a/

        
3. Additional Info:
    All my screenshots are in the /screenshots_project4 folder and in project4-screenshots.pdf.  In order for the assignment the screenshots are lseek_test_output.png, symlink_output.png, block_write_failure.png, many_blocks_output.png, and too_many_blocks_output.png.

    Part 1-lseek:
        The sys_lseek function definition in sysfile.c grabs the two arguments as specified, file descriptor and the offset.  It then gets the struct file* and modifies f->off in that struct in memory.  The filewrite function in file.c is what is responsible for actually writing to a file.  If f->off is beyond the current file size, it fills the holes with zeroes of the offset, then writes the actual data from the user buffer.

        lseek_test from the file lseek_test.c tests the functionality of the lseek system call in two ways.  The first test opens up a text file that says "Hello" and "World" but there is a call to lseek in between to offset the characters.  At first I just opted to print the text file but it only printed "Hello" because of the null terminator offset, so I then opted for a loop that skips the offset holes to concatenate the two strings to output "HelloWorld".  The second way writes "A" and "B" to a file with offsets in between to test the holes that should be filled in between.  I opted to print each byte as integers to better show this, so the output is    "65 0 0 0 0 0 66".

    Part 2-Symbolic Links:
        The sys_symlink system call creates a new inode of type T_SYMLINK as specified in stat.h and stores the target path in that inode.  Adding on to the sys_open system call, I made it so the kernel checks if the inode has a symbolic link.  I also added the possibility of using the flag O_NOFOLLOW, which when not specified, means that sys_open() is to recursively follow the symlink.   Uncomment on line 302 in sysfile.c to see that sys_open is being called correctly.

        I believe I have an issue with sys_open() having a recursive loop that goes on infinitely but despite my best efforts I have not been able to close it.  In theory, symlink_test command from symlink_test.c should create a normal text file that has "HelloXV6" as a string.  The symlink text file created should be tested with and without O_FOLLOW flags to see if it works properly to follow the link to the normal file and read and output "HelloXV6". You can see my debug statements I tried in the screenshot for this part.

    Part 3-Doubly-Indirect blocks:
        I first started implementing doubly-indirect blocks by setting FSIZE in param.h to 250000.  Then, in fs.h, i allocated a block from NDIRECT for the doubly-indirect block and created a constant to hold that called NDOUBINDIRECT, which holds the size of NINDIRECT times NINDIRECT.  MAXFILE is changed to hold the single double-indirect block or also another one for the second test.  In the struct dinodes, I needed to add a pad[] variable to bring the correct amount of bytes to 128 to make sure there was equal ratio of blocks to inodes to avoid crashes. In fs.c, I implemented measures in bmap() and itrunc() to deal with both a single double-indirect block and another.

        The many_blocks command from many_blocks.c tests that 16523 block writes succeeds and that more writes will fail.  Keep in mind for my code, I accidentally had it print out the same index for both success and failure, but if you look at lines 28-34, it will show that failure is printed at i, and success at i + 1, when I meant to have both print at i + 1.  By the time I realized this, I already implemented the next step of the project.  The too_many_blocks command from too_many_blocks.c tests that 32906 block writes succeeds.