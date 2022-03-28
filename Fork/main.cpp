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
            printf("In Parent: Write to pipe getQuoteMessage sent \nMessage:\n%s\n\n", "Get Quote");
            write(pipeParentWriteChildReadfds[WRITE], "Get Quote", sizeof("Get Quote"));

            char ParentReadChildMessage[MAX_PIPE_MESSAGE_SIZE] = { 0 };
            read(pipeParentReadChildWritefds[READ], ParentReadChildMessage sizeof(ParentReadChildMessage));
            printf("In Parent: Read from pipe pipeParentReadChildMessage read\nMessage:\n%s\n\n", ParentReadChildMessage);
        }

        write(pipeParentWriteChildReadfds[WRITE], "Exit", sizeof("Exit"));
        printf("In Parent: Write to pipe ParentWriteChildExitMessage sent\nMessage:\n%s\n\n", "Exit");

        close(pipeParentWriteChildReadfds[WRITE]);
        close(pipeParentReadChildWritefds[READ]);

        printf("\nParent done\n");
    }
    else
    {
        //child process

        // Close the unwanted pipes
        close(pipeParentWriteChildReadfds[WRITE]);
        close(pipeParentReadChildWritefds[READ]);

        time_t t;
        srand((unsigned)time(&t));
        
        do
        {
            char receivedMessage[MAX_PIPE_MESSAGE_SIZE] = {0};

            read(pipeParentWriteChildReadfds[READ], receivedMessage, sizeof(receivedMessage));
            printf("In Child : Read from pipe pipeParentWriteChildMessage read\nMessage:\n%s\n\n", receivedMessage);

            // check message
            char *indexPointer;
            indexPointer = strstr(receivedMessage, "Exit");

            if (indexPointer != NULL)
                break;  // exit message found

            indexPointer = strstr(receivedMessage, "Get Quote");
            if (indexPointer != NULL)
            {
                // Get Quote message found

                // get random quote line choice 0... < noLines
                int randomLineChoice = (rand() % noLines);

                char quoteMessage[1000] = {0};
            }
        } while (true);

        close(pipeParentWriteChildReadfds[READ]);
        close(pipeParentReadChildWritefds[WRITE]);

        printf("Child Done %s \n\n");        
    }
    return EXIT_SUCCESS;
}