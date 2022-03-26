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
    
}