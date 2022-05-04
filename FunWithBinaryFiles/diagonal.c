// Linus Fackler
// LXF210001
// CS3377.501 Systems Programming in UNIX and Other Environments
// Dr. Dollinger
//
// This assignment designs counts the number of diagonal entries

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
// #include <iostream>
// #include <iomanip>

#define LINESIZE 16

//using namespace std;

//use one command line argument
int main(int argc, char *argv[]) {

  if (argc == 1) {
      printf("Usage: diagonal <textstring>\n");
      return -1;
     }

	//create a file so that 16 rows of empty will appear with od -Ax -c command
	int fd = open("diagonal.out", O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);

	char space = '.';
	int length =0;
	int blockcount=0;
	int argNo=0;
	int i=0;
	for (argNo =1; argNo < argc; argNo++) {
	// cout << endl << endl << endl;
	// cout << "argNo " << argNo << endl;
	// cout << "argv["<< argNo << "]" << argv[argNo] << endl;
	// cout << endl;
	length = strlen(argv[argNo]);
	int position;
	//	cout << "blockcount " << blockcount << endl;
	lseek(fd,blockcount, SEEK_SET);
	// cout << "line size " << LINESIZE << endl << endl;

	for (int line =0; line < LINESIZE; line++) {
	// cout << setw(2) << line << " : ";
	for (int column =0; column < LINESIZE; column++) {
	position = lseek(fd,0, SEEK_CUR);
	write(fd,&space, 1);
	}
	//cout << endl;
	}
	//cout << endl << endl;
	

	int argvlength = strlen(argv[argNo]);
	// cout << "argNo	" << argNo << endl;
	// cout << "argv[argNo] " << argv[argNo] << endl;
	// cout << "argvlength " << argvlength << endl;
	//	cout << "blockCount " << blockCount << endl;

	if (argNo % 2 != 0){
	// cout << "lseek " << setw(06) << (blockcount + ((LINESIZE+1)+i)) << " args[" << argNo << "][" << i << "][" << argv[argNo][i] << endl;
	for (int i =0; i < argvlength; i++){
	lseek (fd,(blockcount + (LINESIZE + 1)*i), SEEK_SET);
	write(fd, &argv[argNo][i],1);
	}

	}
	else{
	const int adjust = 15;
	// cout << "adjust " << adjust << endl;

	for(int i =1; i < argvlength +1; i++)
	{
	// cout << "lseek " << setw(06) << (blockcount + (adjust * i)) << " arg[" << argNo << "][" << (i-1) << "]" << argv[argNo][i-1] << endl;
	lseek(fd,(blockcount + (adjust *i)), SEEK_SET);
	write(fd, &argv[argNo][i-1],1);
	}
	}
	blockcount += 256;
	// cout << "blockcount " << blockcount << endl;
	}

	close(fd);

	puts("diagonal.out has been created. Use od -Ax -c diagonal.out to see the contents.\n");
}
