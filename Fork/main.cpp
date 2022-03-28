#include <stdlib.h>
#include <string.h>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <time.h>

using namespace std;

static const char* const QUOTES_FILE_NAME = "quotes.txt";

int const READ      = 0;
int const WRITE     = 1;
int const PIPE_ERROR    = -1;
int const CHILD_PID     =  0;
int const MAX_PIPE_MESSAGE_SIZE     = 1000;
int const MAX_QUOTE_LINE_SIZE       = 1000;

void getQuotesArray(char *lines[], unsigned &noLines)
{
    FILE *inputFilePointer;
    inputFilePointer = fopen(QUOTES_FILE_NAME, "r");
    if (inputFilePointer == NULL)
    {
        perror("\n\nError opening file, I will expire now...\n\n");
        exit(EXIT_FAILURE);
    }

    char input [MAX_QUOTE_LINE_SIZE];
    int size = sizeof(input);
    while (fgets(input, size, inputFilePointer) != NULL)
    {
        lines[noLines] = strdup(input);
        noLines++;
    }
}

int main (int argc, char* argv[])
{
    if (argc != 2)
    {
        perror("\n\nWhere is the number argument, I will expire now...\n\n");
        exit(EXIT_FAILURE);
    }

    int noOfParentMessages2Send = atoi(argv[1]);
    char *lines[1000];
    unsigned noLines;
    srand(clock());
    getQuotesArray(lines, noLines);

    int pipeParentWriteChildReadfds[2], pipeParentReadChildWritefds[2];
    int pipeReturnStatus;
    int pid;

    // create pipes
    pipeReturnStatus = pipe(pipeParentWriteChildReadfds);
    if (pipeReturnStatus == PIPE_ERROR)
    {
        printf("Unable to create pipe pipeParentWriteChildReadfds \n");
        return EXIT_FAILURE;
    }

    printf("\n\n");

    pid = fork();

    if (pid != CHILD_PID)
    {
        // Parent process
        
        // Close the unwanted pipes
        close(pipeParentWriteChildReadfds[READ]);
        close(pipeParentReadChildWritefds[WRITE]);

        for (unsigned i = 0; i < noOfParentMessages2Send; i++)
        {
            printf("In Parent: Write to pipe getQuoteMessage sent \n"); //NEED
            write(pipeParentWriteChildReadfds[WRITE], "Get Quote", sizeof()); //NEED

            char ParentReadChildMessage[MAX_PIPE_MESSAGE_SIZE] = { 0 };
            read(pipeParentReadChildWritefds[READ], ParentReadChildMessage); //NEED
            printf("In Parent: Read from pipe pipeParentReadChildMessage "); //NEED
        }
    }

}