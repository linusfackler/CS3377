#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <iomanip>
#include <iostream>

#define LINESIZE 16

using namespace std;

int main (int argc, char* argv[])
{
    if (argc == 1)
    {
        printf("Usage: diagonal <textstring>\n");
        return -1;
    }

    // create a file so that 16 rows of empty will appear with od -c command
    int fd = open("diagonal2.out", O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);

    char space = '.';

    int length = 0;
    int blockcount = 0;

    for (int argNo = 1; argNo < argc; argNo++)
    {
        cout << endl << endl << endl;

        cout << "argNo " << argNo << endl;
        cout << "argv[" << argNo << "]" << argv[argNo] << endl;
        length = strlen(argv[argNo]);
        cout << "length " << length << endl;
        cout << endl;
        int position;
        cout << "blockCount " << blockcount << endl;
        lseek(fd, blockcount, SEEK_SET);
        cout << "line size " << LINESIZE << endl << endl;

        for (int line = 0; line < LINESIZE; line++)
        {
            cout << setw(2) << line << " : ";
            for (int column = 0; column < LINESIZE; column++)
            {
                position = lseek(fd, 0, SEEK_CUR);
                cout << setw(04) << position << space << ' ';
                write(fd, &space, 1);
            } 
            cout << endl;
        }

        cout << endl << endl;

        // each line of od outputs 16 characters
        // so, to make the output diagonal, we will use 0, 17, 34, ...

        int argvlength = strlen(argv[argNo]);
        cout << "argNo       " << argNo         << endl;
        cout << "argv[argNo] " << argv[argNo]   << endl;
        cout << "argvlength  " << argvlength    << endl;
        cout << "blockCount  " << blockcount    << endl;

        // odd left to right
        if (argNo % 2 != 0)
        {
            for (int i = 0; i < argvlength; i++)
            {
                cout << "lseek" << setw(06) << (blockcount + (LINESIZE + 1) * i)
                    << " argv[" << argNo << "][" << i << "] " << argv[argNo][i] << endl;
                lseek(fd, (blockcount + (LINESIZE + 1) * i), SEEK_SET);
                write(fd, &argv[argNo][i], 1);
            }
        }
        // even left to right
        
        else
        {
            const int adjust = 15;
            cout << "adjust " << adjust << endl;

            for (int i = 1; i < argvlength + 1; i++)
            {
                cout << "lseek " << setw(06) << (blockcount + (adjust * i)) << " argv[" << argNo 
                    << "][" << (i-1) << "] " << argv[argNo][i-1] << endl;
                lseek(fd, (blockcount + (adjust * i)), SEEK_SET);
                write(fd, &argv[argv][i-1], 1);
            }
        }

        blockcount += 256;
        cout << "blockCount " << blockcount << endl;
    }

    close(fd);

    puts("diagonal2.out has been created.\nUse od -Ad-c diagonal.out to see the contents.\n");
}