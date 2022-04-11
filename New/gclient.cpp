#include <sstream>
#include <stdio.h>

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

using namespace std;

const int READ_MAX_LEN = 100;
const int NO_OF_LETTER_GUESS_MAX = 12;

int main(int argc, char* argv[])
{
    if (argc != 1)
    {
        cout << "Usage: gclient " << endl;
        exit(EXIT_FAILURE);
    }

    string srd_cwr_req_np_str = "./tmp/srd_cwr_req_np";
    mkfifo(srd_cwr_req_np_str.c_str(), 0600);
    chmod (srd_cwr_req_np_str.c_str(), 0622);
    int    srd_cwr_req_np_fd = open(srd_cwr_req_np_str.c_str(), O_WRONLY);

    string swr_crd_np_str = "./tmp/swr_crd_np-";
    int i = getpid();
    string s;
    stringstream out;
    out << i;
    s = out.str();
    swr_crd_np_str += s;
    write(srd_cwr_req_np_fd, swr_crd_np_str.c_str(), swr_crd_np_str.size() + 1);

    mkfifo(swr_crd_np_str.c_str(), 0600);
    chmod (swr_crd_np_str.c_str(), 0622);
    int    swr_crd_np_fd = open(swr_crd_np_str.c_str(), O_RDONLY);

    char clientno_ary[READ_MAX_LEN] = { 0 };
    read(swr_crd_np_fd, clientno_ary, READ_MAX_LEN);

}