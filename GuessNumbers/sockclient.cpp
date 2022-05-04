// Linus Fackler
// LXF210001
// CS3377.501 Systems Programming in UNIX and Other Environments
// Dr. Dollinger
//
// This program lets the user guess a number between 0 and 100
//
// This is the client for the socket

#include <string>
#include <iostream>
#include <cstdlib> 
#include <sstream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

const int    MESSAGE_MAX_SIZE = 1024;
const int    ERROR       = -1;
const int    SOCK_PORT   = 6969;
const string LOCAL_HOST_ADDRESS = "127.0.0.1";

const int MAX_GUESS_LIMIT   = 100;
const int MAX_NO_TRIES      = 20;

int main(int argc, char* argv[])
{
    int         socketServerId, status;
    sockaddr_in server;
    string      messageStr;

    //Create socket
    socketServerId = socket(AF_INET, SOCK_STREAM, 0);
    if (socketServerId == ERROR)
    {
        cerr << "Failure: Create Socket" << endl;
        exit(EXIT_FAILURE);
    }

    server.sin_addr.s_addr = inet_addr(LOCAL_HOST_ADDRESS.c_str());
    server.sin_family      = AF_INET;
    server.sin_port        = htons(SOCK_PORT);

    //Connect to remote server
    status = connect(socketServerId, (struct sockaddr*) &server, sizeof(server));
    if (status == ERROR)
    {
        perror("Failure: connect failed. Error");
        exit(EXIT_FAILURE);
    }

    cout << endl << endl << "Try to guess a number between 1 and " << MAX_GUESS_LIMIT << endl << endl;
    int noOfTries = 0;
    int guessNo;

    //keep communicating with server
    do
    {
        cout << "Current Try Number : " << ++noOfTries << endl;
        cout << "Enter number guess : " << endl << endl;
        cin >> guessNo;

        stringstream gnoss;
        gnoss << guessNo;
        string guessNoStr = gnoss.str();

        status = write(socketServerId, guessNoStr.c_str(), guessNoStr.size() + 1);

        if (status == ERROR)
        {
            cout << "Failure: Client Write failed" << endl;
            exit(EXIT_FAILURE);
        }

        char receiveMessage[MESSAGE_MAX_SIZE] = {0};
        status = read(socketServerId, receiveMessage, MESSAGE_MAX_SIZE);

        if (status == ERROR)
        {
            cout << "Failure: read failed" << endl;
            exit(EXIT_FAILURE);
        }

        string receiveMessageStr = receiveMessage;
        cout << endl << "Server reply :" << endl << endl << receiveMessageStr << endl << endl;

        if (receiveMessageStr.find("Win") != string::npos)
            break;

        if (noOfTries == MAX_NO_TRIES) {
            cout << endl << "The maximum number of tries " << MAX_NO_TRIES << " is done." << endl;
            status = write(socketServerId, "Send Random Number", sizeof("Send Random Number")+1);
            if(status = ERROR)
            {
                cout << "Failure: Client Write failed" << endl;
                exit(EXIT_FAILURE);
            }

            char randomNoMessage[MESSAGE_MAX_SIZE];
            status = read(socketServerId, randomNoMessage, MESSAGE_MAX_SIZE);
            if (status == ERROR)
            {
                cout << "Failur: read failed" << endl;
                exit(EXIT_FAILURE);
            }
            int randomno = stoi(randomNoMessage);

            cout << "The random number is : " << randomno << endl << endl;
            cout << "Game is over " << endl << endl;
            break;            
        }
    } while (true);

    close(socketServerId);
    exit(EXIT_SUCCESS);
}