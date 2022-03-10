// #include <iostream>
// #include <string>
// #include <fstream>
#include <unistd.h>
#include <string.h>

#define LINESIZE 16

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
        cout << "blockCount "

    }
}