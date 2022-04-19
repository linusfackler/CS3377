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