#include <string>
#include <fstream>
#include <iostream>
#include <stdint.h>

#include <stdlib.h>
#include <string.h>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

using namespace std;

int const READ   = 0;
int const WRITE  = 1;
int const PIPE_ERROR = -1;
int const CHILD_PID  =  0;
int const MAX_PIPE_MESSAGE_SIZE = 1000;
int const EXECVP_FAILED = -1;


void CreateArg1FileWithArg2RandomNumbersArg3RandomRange
     (string randomFileNameStr, unsigned noRandomNumbersUns, unsigned  randomRange)
{

    ofstream outfileStream (randomFileNameStr);

    if (outfileStream.fail())
    {
        cerr<<"Oh my Goodness, Error opening file Randon Numbers File : " << randomFileNameStr << endl <<
              "I will expire now..." << endl<<endl;
        exit(EXIT_FAILURE);
    }

    // seed the random number generator
    time_t t; srand((unsigned)time(&t));


    for(unsigned i =0; i < noRandomNumbersUns; ++i)
        outfileStream << ( ( rand() % randomRange) + 1 ) << endl;

    outfileStream.close();
    }

int main(int argc, char* argv[])
{

    if (argc != 4)
    {
        cerr<<endl<<endl<<
            "Wrong arguments"<<endl<<
            "Usage:"<< endl << 
            "forkexecvp <file to create> <amount of random numbers> <random number range>" << endl <<
            "Example:"<<endl <<
            "forkexecvp randomnumber 10000 100"
            <<endl<<endl;
        exit(EXIT_FAILURE);
    }

    string   randomFileNameStr  = argv[1];
    unsigned noRandomNumbersUns = atoi(argv[2]),
             randomRange        = atoi(argv[3]);
    
    cout << endl << argv[0] << " " << randomFileNameStr << " " << noRandomNumbersUns << " " << randomRange << endl;

    CreateArg1FileWithArg2RandomNumbersArg3RandomRange
    (randomFileNameStr, noRandomNumbersUns, randomRange);
    
    pid_t pid;
    int pipeParentWriteChildReadfds[2];
    int pipeReturnStatus;

    string messages[] = {"sum", "average", "greatest", "least"};
    int noOfMessages = sizeof(messages) / sizeof(messages[0]);
    
    for (int childProcessNo = 0; childProcessNo < noOfMessages; ++childProcessNo)
    {

        pipeReturnStatus = pipe(pipeParentWriteChildReadfds);
        if (pipeReturnStatus == PIPE_ERROR)
        {
            cerr << "Unable to create pipe pipeParentWriteChildReadfds" << endl;
            return EXIT_FAILURE;
        }

        pid_t forkpid = fork();

        if (forkpid < 0)
        {
            cerr<<endl<<endl<<"Oh my goodness, fork failed"<< endl << endl;
            exit(EXIT_FAILURE);
        }

        if (forkpid != CHILD_PID)
        {
            // parent
            close(pipeParentWriteChildReadfds[READ]);

            cout << "Parent pid : " << getpid() << " to Child Process No : " 
                 << childProcessNo << endl <<
                 "Send Message : " << messages[childProcessNo]  << endl;

            write(pipeParentWriteChildReadfds[WRITE], 
                  messages[childProcessNo].c_str(), 
                  sizeof(messages[childProcessNo].c_str()));
         
        }
        else
        { 
            // child
            close(pipeParentWriteChildReadfds[WRITE]);

            char pipeReadMessage[MAX_PIPE_MESSAGE_SIZE] = {0};
            read(pipeParentWriteChildReadfds[READ], pipeReadMessage, 
                sizeof(pipeReadMessage));
            
            cout << "Child pid : " << getpid() << " Child Process No : " << childProcessNo <<
                "Received Message : " << pipeReadMessage <<  endl;
                       
            // execvp
            char * arglist[] = { (char *)"./calculate", pipeReadMessage, 
                                 (char*)randomFileNameStr.c_str(), NULL };
            
            cout << "Child pid : " << getpid() << " Child Process No : " << childProcessNo << endl <<
                "execvp(" << arglist[0] << ", ./calculate, " << randomFileNameStr.c_str() << ", NULL)" << endl;

            //replace stdout to the answer file named with pipeReadMessage
            string answerFileName = "answer"; answerFileName += pipeReadMessage; answerFileName += ".txt";
            freopen(answerFileName.c_str(), "w", stdout);

            close(pipeParentWriteChildReadfds[READ]);

            int statusCode = execvp(arglist[0], arglist);

            if (statusCode == EXECVP_FAILED)
            {
                cout << "Child pid : " << getpid() << " Child Process No : " << childProcessNo << endl <<
                    "execvp(" << arglist[0] << ", ./calculate, " << randomFileNameStr.c_str() << ", NULL)" << endl;
                cout << "****** Failed *******" << endl;
                exit(EXIT_FAILURE);
            }

        }

    }

    cout << "Parent pid: " << getpid() << " Start - Wait for all calculate children to finish" << endl;
    while (wait(NULL) > 0);
    cout << "Parent pid: " << getpid() << " Done  - Wait for all calculate children to finish" << endl << endl;

    close(pipeParentWriteChildReadfds[WRITE]);

    cout << "Parent pid : " << getpid() << " Use execvp() cat to display answer files: " << endl;

    for (int childProcessNo = 0; childProcessNo < noOfMessages; ++childProcessNo)
    {

        pid_t forkpid = fork();

        if (forkpid < 0)
        {
            cerr << endl << endl << "Oh my goodness, fork failed" << endl << endl;
            exit(EXIT_FAILURE);
        }

        if (forkpid == CHILD_PID)
        {
            //child code
            
            string answerFileName = "answer"; answerFileName += messages[childProcessNo]; answerFileName += ".txt";

            char* arglist[] = { (char*)"cat", (char*)answerFileName.c_str(), NULL };

            cout << "Child pid : " << getpid() << endl <<
                 "cat " << answerFileName << endl;

            int statusCode = execvp("cat", arglist);

            if (statusCode == EXECVP_FAILED) {
                cout << "Child pid : " << getpid() << " Child Process No : " << childProcessNo << endl <<
                    "execvp(" << arglist[0] << ", ./calculate, " << randomFileNameStr.c_str() << ", NULL)" << endl;
                cout << "****** Failed *******" << endl;
                exit(EXIT_FAILURE);
            }

        }

    }
    
    cout << "Parent pid: " << getpid() << " Start - Wait for all display answer file children to finish" << endl;
    while (wait(NULL) > 0);
    cout << "Parent pid: " << getpid() << " Done  - Wait for all display answer file children to finish" << endl << endl;

    return(EXIT_SUCCESS);
}
