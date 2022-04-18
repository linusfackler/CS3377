#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <vector>
#include <iterator>
#include <numeric>
#include <algorithm>

using namespace std;

const int READ_MAX_LEN = 100;

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        cout << "Problem" << endl;
        exit(EXIT_FAILURE);
    }

    vector<string> vectorStrings;
    ifstream FileStringStream(argv[1]);
    istream_iterator<string> inputIt(FileStringStream);

    copy(inputIt, istream_iterator<string>(), back_inserter(vectorStrings));
    
    int NoOfElements = vectorStrings.size();
    static int clientcount = 0;

    do
    {
        string srd_cwr_req_np_str = "./tmp/srd_cwr_req_np";
        mkfifo(srd_cwr_req_np_str.c_str(), 0600);
        chmod (srd_cwr_req_np_str.c_str(), 0622);
        int    srd_cwr_req_np_fd  = open(srd_cwr_req_np_str.c_str(), O_RDONLY);

        char swr_crd_np_ary[READ_MAX_LEN] = { 0 };
        read(srd_cwr_req_np_fd, swr_crd_np_ary, READ_MAX_LEN);
        string swr_crd_np_str(swr_crd_np_ary);

        close (srd_cwr_req_np_fd);
        unlink(srd_cwr_req_np_str.c_str());

        mkfifo(swr_crd_np_str.c_str(), 0600);
        chmod (swr_crd_np_str.c_str(), 0622);
        int    swr_crd_np_fd = open(swr_crd_np_str.c_str(), O_WRONLY);

        time_t t;
        srand((unsigned)time(&t));
        int    randomIndexChoice = (rand() % NoOfElements);
        string randomword_str = vectorStrings[randomIndexChoice];
        string guessword_str(randomword_str.length(), '-');

        clientcount++;
        
        if (fork() == 0)
        {
            stringstream ss;
            ss << clientcount;
            string str = ss.str();
            string clientcount_str = ss.str();
            write(swr_crd_np_fd, clientcount_str.c_str(), clientcount_str.size() + 1);

            sleep(1);

            write(swr_crd_np_fd, randomword_str.c_str(), (randomword_str.size() + 1));

            string srd_cwr_np_str = "./tmp/srd_cwr_np-";
            int i = getpid();
            string s;
            stringstream out;
            out << i;
            s = out.str();
            srd_cwr_np_str += s;
            write(swr_crd_np_fd, srd_cwr_np_str.c_str(), srd_cwr_np_str.size() + 1);

            mkfifo(srd_cwr_np_str.c_str(), 0600);
            chmod (srd_cwr_np_str.c_str(), 0622);
            int    srd_cwr_np_fd = open(srd_cwr_np_str.c_str(), O_RDONLY);

            do
            {
                write(swr_crd_np_fd, guessword_str.c_str(), (guessword_str.size() + 1));

                char guessletter_ary[READ_MAX_LEN] = { 0 };
                read(srd_cwr_np_fd, guessletter_ary, READ_MAX_LEN);

                for (int i = 0; i < randomword_str.length(); i++)
                    if (randomword_str[i] == guessletter_ary[0])
                        guessword_str[i] == guessletter_ary[0];
                
            } while (true);
            
            exit(EXIT_SUCCESS);
        }

    } while (true);
    
    exit(EXIT_SUCCESS);
}