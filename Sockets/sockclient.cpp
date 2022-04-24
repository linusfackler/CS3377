// number guess client

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
const int    SOCK_PORT   = 8888;
const string LOCAL_HOST_ADDRESS = "127.0.0.1";

const int MAX_GUESS_LIMIT   = 100;
const int MAX_NO_TRIES      = 12;

int main(int argc, char* argv[])
{
    int         socketServerId, status;
    sockaddr_in server;
    string      messageStr;
}