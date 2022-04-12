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
    string clientno_str(clientno_ary);

    char randomWordAry[READ_MAX_LEN] = { 0 };
    read(swr_crd_np_fd, randomWordAry, READ_MAX_LEN);
    string randomword_str(randomWordAry);

    char srd_cwr_np_ary[READ_MAX_LEN] = { 0 };
    read(swr_crd_np_fd, srd_cwr_np_ary, READ_MAX_LEN);
    string srd_cwr_np_str(srd_cwr_np_ary);

    mkfifo(srd_cwr_np_str.c_str(), 0600);
    chmod (srd_cwr_np_str.c_str(), 0622);
    int    srd_cwr_np_fd = open(srd_cwr_np_str.c_str(), O_WRONLY);

    int noOfTries = 0;

    cout << endl << endl << "Game Start" << endl <<
        "You have " << NO_OF_LETTER_GUESS_MAX << " letter guesses to win" << endl << endl;

    do
    {
        char guessWordAry[READ_MAX_LEN] = { 0 };
        read(swr_crd_np_fd, guessWordAry, READ_MAX_LEN);
        string guessword_str(guessWordAry);

        // Determine if done:
        if (guessword_str == randomword_str)
        {
            cout << endl << endl << "You Win!" << endl << endl;
            break;
        }

        if (noOfTries > NO_OF_LETTER_GUESS_MAX)
        {
            cout << endl << endl << "Out of tries : " << noOfTries - 1 << " allowed"
                << endl << "The word is: " << randomword_str << endl << endl;
            break;
        }

        cout << endl;
        cout << "Client : " << clientno_str << endl <<
            "No of tries : " << noOfTries << endl <<
            "(Guess) Enter a letter in the word : " << guessword_str << endl;
        cout << endl;

        char letterGuess_char = ' ';
        cin >> letterGuess_char;
        string letterGuess_str = " ";
        letterGuess_str[0] = letterGuess_char;

        write(srd_cwr_np_fd, letterGuess_str.c_str(), letterGuess_str.size() + 1);

        noOfTries++;

    } while (true);
    

}