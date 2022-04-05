// Linus Fackler
// LXF210001
// CS3377.501 Systems Programming in UNIX and Other Environments
// Dr. Dollinger
//
// This assignment designs forks to create multiple child processes.
// Each will use information sent by the parent over a pipe to execute
// the except(). This will overlay each child using execvp() to calculate.

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <iterator>
#include <numeric>
#include <algorithm>
using namespace std;

int main(int argc, char* argv[])
{
	if (argc != 3) {
		cout << endl << endl <<
		"Wrong arguments" << endl <<
		"Usage: " << endl <<
		"calculate < sum | average | greatest | least > filenae.txt" << endl <<
		"Example:" << endl << 
		"calculate sum filerandom.txt" << endl << endl;
		
		exit(EXIT_FAILURE);
	}
	
	cout << argv[0] << " " << argv[1] << " " << argv[2] << endl;
	vector<int> vectorInts;
	
	ifstream randomNos(argv[2]);
	istream_iterator<int> inputIt(randomNos);
	
	copy(inputIt, istream_iterator<int>(), back_inserter(vectorInts));
	
	string command = argv[1];
	
	if (command == "sum")
		cout << "Sum		: " << accumulate(vectorInts.begin(), vectorInts.end(), 0) << endl;
	else if (command == "average")
		cout << "Average	: " << (accumulate(vectorInts.begin(), vectorInts.end(), 0) ) / ((float) vectorInts.size() ) << endl;
	
	else if (command == "least") {
		cout << "Least	: " << *(min_element(vectorInts.begin(), vectorInts.end() )) << endl;
	}
	
	else if (command == "greatest")
		cout << "Greatest : " << *(max_element(vectorInts.begin(), vectorInts.end()) ) << endl;
	
	return(EXIT_SUCCESS);
}
