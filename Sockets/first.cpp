#include <iostream>
#include <string>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/inet.h>
#include <unistd.h>

using namespace std;

const int MESSAGE_MAX_SIZE = 1024;
const int ERROR = -1;
const int SOCK_PORT = 8888;
const int LISTEN_QUEUE_SIZE = 3;
const int HIGHEST_NO = 100;

int main(int argc, char* argv[])
{
    int    socket_desc, client_sock, read_size, addrsize, status;
    struct sockaddr_in server, client;

    //Create socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == ERROR)
    {
        cerr << "Failure: Create socket" << endl;
        exit (EXIT_FAILURE);
    }

    //Setup the sockaddr_in structure
    server.sin_family       = AF_INET;
    server.sin_addr.s_addr  = INADDR_ANY;
    server.sin_port         = htons(SOCK_PORT);

    //Bind
    status = bind(socket_desc, (struct sockaddr*)&server, sizeof(server));
    if (status == ERROR)
    {
        cout << "Failure: Socket bind " << endl;
        exit(EXIT_FAILURE);
    }

    listen(socket_desc, LISTEN_QUEUE_SIZE);

    addrsize = sizeof(struct sockaddr_in);

    client_sock = accept(socket_desc, (struct sockaddr*) &client, (socklen_t*) &addrsize);
    if (client_sock == ERROR)
    {
        cout << "Failure: accept" << endl;
        exit(EXIT_FAILURE);
    }

    //create a random number and associated random number string
    time_t t;
    srand((unsigned)time(&t));
    int randomno = (rand() % HIGHEST_NO + 1);
    stringstream rnoss;
    rnoss << randomno;
    string randomnostr = rnoss.str();

    string  playresultstr;
    int     guessno;
    string  guessnostr;

    // Receive a guess no messages frm client
    do
    {
        char clientGuessNoMessage[MESSAGE_MAX_SIZE] = {0};
        read_size = read(client_sock, clientGuessNoMessage, MESSAGE_MAX_SIZE);

        guessnostr = clientGuessNoMessage;
        cout << guessnostr << endl;
        if (guessnostr == "0"){
            break;
        }
        else if (read_size > 0){
            string clientGuessNoMessagestr(clientGuessNoMessage);
	    guessno = stoi(clientGuessNoMessage);
           // guessno = stoi(clientGuessNoMessage);
            guessnostr = clientGuessNoMessage;
            if (randomno == guessno){
                playresultstr = "You Win !! \n The number is : " + randomnostr;
            }
            else if (randomno > guessno){
                playresultstr = "The number is greater than: " + guessnostr;
            }
            else if (randomno < guessno){
                playresultstr = "The number is less than : " + guessnostr;
            }

            write(client_sock, playresultstr.c_str(), playresultstr.size() + 1);
        }
        else if (read_size == 0){
            cout << "Client disconnected" << endl;
            fflush(stdout);
            break;
        }
        else if (read_size == ERR){
            cout << "Failure: receive" << endl;
            exit(EXIT_FAILURE);
        }
    } while (/* condition */);
    
}