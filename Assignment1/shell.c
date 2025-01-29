/* 
  Mohammad Omar Zahir - zahirm1
  Mahad Aziz - azizm17
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include  <sys/types.h> /* This header file has the definition for pid_t type*/

#define MAX_LINE 80 /* The maximum length command */
//Stores commands for history
char history[5][256];
//Stores various arrays alternatively used for storing the input
char input[256];
char inp[256];
char inp2[256];
char inp3[256];
//Stores the tokens that are broken down by the strtok() function for our input
char *tokens;
//Temporary arrays for storing values/inputs
char temp1[256];
char temp2[256];
//Stores the index values of history up until 4
int k = 0;
int f = 0;
int g = 0;
int h = 0;
//Keeps track of total commands
int total = 0;

/* This function is responsible for creating the child process and determining whether or not to run the command concurrently. */
int execute(char *args[MAX_LINE/2 + 1], int i){ 
    //Checks if the final character of the user's command is &
    if(strcmp(args[i], "&") == 0){
        //Create child process
        pid_t pid;    
        pid = fork();
        args[i] = NULL;
        //Child executes command
        if(pid == 0){
            execvp(args[0], args);
            return 0;
        }
        //Parent concurrently proceeds
    }
    else{
        //Create child process
        pid_t pid;    
        pid = fork();
        //Child executes command
        if(pid == 0){
            execvp(args[0], args);
            return 0;
        }
        //Parent will wait for all children that may have been forked to finish processing
        //The wait(NULL) != -1 allows us to avoid unexpected compatibility behavior
        else{
            while (wait(NULL) != -1) {}   
        }
    }
    return 0;
}

/* This function is responsible for adding the user's entered command into the history array. */
int addHistory(char inp[256]){
    //Adds input to the history array depending on the values of history index if there are less than 5 user commands already in the array
    if (k <= 4){
        strcpy(history[k],inp);
        k++;
    }
    //Uses logic to shift all elements in history one spot down if there are 5 or more user commands in the history array
    else {
        //Stores the final element of history in a temp and stores the current user input into the final element of history
        strcpy(temp1,history[4]);
        strcpy(history[4],inp);
        //Shift all the elements in history array down an index and also removes the user command that was initalliy the first index of history
        for (int j=3; j>=0; j--) {
            strcpy(temp2,history[j]);
            strcpy(history[j],temp1);
            strcpy(temp1,temp2);
        }
    }
    return 0;
}

/* This function is responsible for displaying the user's command history stored in the history array. */
int showHistory(void){
    f = k;
    g = total;
    //Prints the values in history depending on whether the total is greater or less than 5
    for (int j=k-1; j>=0;j--) {
        // Prints the commands in history if there are less than 5
        if (total < 5)
        {
            printf("%d %s\n", f, history[j]);
            f--;
        }
        // Prints the commands in history if there are 5 or more
        else
        {
            printf("%d %s\n", g, history[j]);
            g--;
        }
    }
}

/* This main function is responsible for taking user input and determining which command the user wants to use. */
int main(void)
{
    char *args[MAX_LINE/2 + 1]; /* command line arguments */
    int should_run = 1; /* flag to determine when to exit program */

    while (should_run) {
        char *args[MAX_LINE/2 + 1]; /* command line arguments */
        printf("osh>");
        //Takes the input, storing it in input
        fgets(input, 256, stdin);
        //Ignores if nothing inputted
        if (strcmp(input, "\n") == 0){
            continue;
        }
        strcpy(input,strtok(input, "\n"));
        //Exits if exit is inputted
        if (strcmp(input, "exit") == 0){
            break;
        }
        //Display history if history is inputted
        else if (strcmp(input, "history") == 0) {
            showHistory();
            total --;
        }
        //Repeat previous user entered command if !! is inputted
        else if (strcmp(input, "!!") == 0){
            //Checks if there are any previous commands
            if (total == 0){
                printf("No command in history.\n");
                total --;
            }
            //Removes most recent command from history, breaks it down into the token array, giving it to execute function, and adds it to history
            else{
                //Store the last command in history into temporary variables
                strcpy(inp3, history[k-1]);
                strcpy(inp2, history[k-1]);
                //Breaks down the user's command by parsing the string when a " " appears and stores this in an array that holds each argument
                tokens = strtok(inp2, " ");
                int i = 0;
                while(tokens != NULL) {
                    args[i] = tokens;
                    tokens = strtok(NULL, " ");
                    i = i+1;
                }
                //For the user command to execute we add NULL to the end of args array
                args[i] = NULL;
                //Adds the user command to history and then also sends the command to get executed
                addHistory(inp3);
                execute(args, i-1);
            }
        }
        else{
            //Breaks down input into tokens
            strcpy(inp, input);
            tokens = strtok(input, " ");
            int i = 0;
            while(tokens != NULL) {
                args[i] = tokens;
                tokens = strtok(NULL, " ");
                i = i+1;    
            }
            args[i] = NULL;
            //Checks if the input is to repeat a command from history
            if (strcmp(args[0], "!") == 0){
                //If no number given
                if (args[1] == NULL){
                    printf("No number given!\n");
                    total--;
                }
                //If number is not applicable
                else if(atoi(args[1]) <= 0){
                    printf("No such command in history.\n");
                    total --;
                }
                //Checks if input is a valid command
                else if (atoi(args[1]) <= total && atoi(args[1]) >= total-4){
                    //Performs arithmetic logic to determine the correct index of history to refernce
                    if (total < 5){
                        h = atoi(args[1]);
                    }
                    else{
                        h = 5 - (total - atoi(args[1]));
                    }
                    //Retrieves, breaks down, executes the respective command from history, and stores the command again into history
                    strcpy(inp3, history[h-1]);
                    strcpy(inp2, history[h-1]);
                    tokens = strtok(inp2, " ");
                    int i = 0;
                    while(tokens != NULL) {
                        args[i] = tokens;
                        tokens = strtok(NULL, " ");
                        i = i+1;    
                    }
                    args[i] = NULL;
                    execute(args, i-1);
                    addHistory(inp3); 
                }
                else{
                    printf("No such command in history.\n");
                    total --;
                }
            }
            //Does this if it is any other regular command (not special ! commands)
            else{
                execute(args, i-1);
                addHistory(inp); 
            }
        }
        // Increment total number of commands user has entered
        total++;
        //Flushes output
        fflush(stdout);
    }
    return 0;
}