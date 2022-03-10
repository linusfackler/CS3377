#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define LINESIZE 16
#include <string.h>
#include <iomanip>
#include <iostream>

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

        for (int line = 0; line < LINESIZE; line++)
        {
            cout << setw(2) << line << " : ";
            for (int column = 0; column < LINESIZE; column++)
            {
                position = lseek(fd, 0, SEEK_CUR);
                cout << setw(04) << position ++ space << " ";
                write(fd, &space, 1);
            } 
            cout << endl;
        }

        cout << endl << endl;

        int argvlength = strlen(argv[argNo]);
        cout << "argNo       " << argNo         << endl;
        cout << "argv[argNo] " << argv[argNo]   << endl;
        cout << "argvlength  " << argvlength    << endl;
        cout << "blockCount  " << blockdount    << endl;

        // odd left to right
        if (argNo % 2 != 0)
        {
            for (int i = 0; i < argvlength; i++)
            {
                cout << "lseek" << setw(06) << (blockcount + )
            }
        }



    }
}