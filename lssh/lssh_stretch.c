#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>

#define PROMPT "lambda-shell$ "

#define MAX_TOKENS 100
#define COMMANDLINE_BUFSIZE 1024
#define DEBUG 1  // Set to 1 to turn on some debugging output, or 0 to turn off

/**
 * Parse the command line.
 *
 * YOU DON'T NEED TO MODIFY THIS!
 * (But you should study it to see how it works)
 *
 * Takes a string like "ls -la .." and breaks it down into an array of pointers
 * to strings like this:
 *
 *   args[0] ---> "ls"
 *   args[1] ---> "-la"
 *   args[2] ---> ".."
 *   args[3] ---> NULL (NULL is a pointer to address 0)
 *
 * @param str {char *} Pointer to the complete command line string.
 * @param args {char **} Pointer to an array of strings. This will hold the result.
 * @param args_count {int *} Pointer to an int that will hold the final args count.
 *
 * @returns A copy of args for convenience.
 */
char **parse_commandline(char *str, char **args, int *args_count)
{
    char *token;
    
    *args_count = 0;

    token = strtok(str, " \t\n\r");

    while (token != NULL && *args_count < MAX_TOKENS - 1) {
        args[(*args_count)++] = token;

        token = strtok(NULL, " \t\n\r");
    }

    args[*args_count] = NULL;

    return args;
}

/**
 * Main
 */
int main(void)
{
    // Holds the command line the user types in
    char commandline[COMMANDLINE_BUFSIZE];

    // Holds the parsed version of the command line
    char *args[MAX_TOKENS];

    // How many command line args the user typed
    int args_count;

    // Shell loops forever (until we tell it to exit)
    while (1) {
        // Print a prompt
        printf("%s", PROMPT);
        fflush(stdout); // Force the line above to print

        // Read input from keyboard
        fgets(commandline, sizeof commandline, stdin);

        // Exit the shell on End-Of-File (CRTL-D)
        if (feof(stdin)) {
            break;
        }

        // Parse input into individual arguments
        parse_commandline(commandline, args, &args_count);

        if (args_count == 0) {
            // If the user entered no commands, do nothing
            continue;
        }

        // Exit the shell if args[0] is the built-in "exit" command
        if (strcmp(args[0], "exit") == 0) {
            break;
        }

        #if DEBUG

        // Some debugging output

        // Print out the parsed command line in args[]
        for (int i = 0; args[i] != NULL; i++) {
            printf("%d: '%s'\n", i, args[i]);
        }

        #endif
        
        /* Add your code for implementing the shell's logic here */
        
        // Task 1
        // Fork a child process to run the new command.
        int rc = fork();

        if (rc<0) {
            fprintf(stderr, "ERROR: Fork Failed\n");
            exit(1);
        }
        // Exec the command in the child process.
        else if (rc == 0) {

        //Task 2
        // 1. Check to make sure they've entered 2 total arguments
            if (strcmp(args[0], "cd") == 0) {
                if(args_count != 2 ){
                    printf("cd to where?\n");
                } else {
        // 2. Run the system call chdir() on the second argument to change directories
                    int cd_check = chdir(args[1]);
        // 3. Error check the result of chdir(). If it returns -1, meaning an error
        //    occurred, you can print out an error message with:
        //       perror("chdir"); // #include <errno.h> to use this
                    if (cd_check == -1) {
                        perror("chdir");
                    }
                }
        // 4. Execute a continue statement to short-circuit the rest of the main loop.
                    continue;                
            }

            execvp(args[0], args);
        }
        // Parent process waits for child to complete.
        else {
            waitpid(rc, NULL, 0);
        }
    }

    return 0;
}