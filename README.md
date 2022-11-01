Readme:

Our program consists of 4 major parts, printing the prompt (when PROMPT macro is 
indicated), reading and parsing user input, handling redirections and handling 
commands. The only method except for the main method is the parse method.

Our program first enters the outer while loop, which is the parent process. The
first thing is to check whether the user indicated the PROMPT macro, if yes,
print the prompt out. Then, the program initialize the buffer, *tokens, *argv
and *redirect, and buf as character arrays and pointers to character arrays.
After this, we initialize a global variable ssize_t bytesRead, which we use to
store the value returned by calling read(), reporting an error when it returns
-1, exiting the shell when it returns 0 (which means that the user uses ctrl-D),
and parse the input otherwise. Parse is a method that will returns 1 if there
is an error within parsing and 0 if the input is successfully parsed. Parse()
will tokenize the input, and if nothing is tokenized, return 1 and continue
into the next iteration (handle the case when user hits enter). Then parse()
checks for the correct use the redirection symbols, reporting error if missing
input/output file, and proceed to store the redirection symbol along with the
file redirecting to into *redirect, deleting them in argv. The final step is to
check multiple input/output files by checking the number of each kind of 
redirection symbols in *redirect. Next, in main(), we first check the condition
for built-in command exit(), and exiting the parent process if so. Then, we
initiate a child process, and start with handling redirections. We iterate from 
0 to 3, each iteration increment by 2, in order to handle the case when there 
are two redirection symbols in the line. Then we check for the redirection 
symbols, close() the corresponding file descriptor and open the file redirected
to. For the cases of ">" and ">>", we make use of flag O_CREAT, and mode 0666,
so that we create a file when it does not exist for output. After this, we come
to handle commands. For the 3 build in commands, we use syscalls to implement,
for the rest, we use execv(), and changing the command to its binary name form
after taking out its full name to input in execv(). The above is a summary of 
the entire program. To compile the program, we can use the makefile we write,
typing make clean all to make sh.c for both with prompt and no prompt, or
we can make individually.

No bugs are detected by now.

Distribution of work:
We work on the functionality together. Readme is completed by zguo47, comments
are completed by xwang259. 