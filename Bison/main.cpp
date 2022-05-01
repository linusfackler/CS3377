// Linus Fackler
// LXF210001
// CS3377.501 - Systems Programming in UNIX and Other Environments
// Dr. Dollinger
//
// This program reads a string from a certain input file and checks
// the number of distinct pairs of indices x < y  at which there is
// the pattern (( at index x and the pattern )) at index y

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

using namespace std;


int main(int argc, char** argv)
{
    cout.imbue(locale(""));
    // condition cout set to local digit seperation commas (USA)

    string INPUT_FILE_NAME = "bisonsearchin.txt";
    string OUTPUT_FILE_NAME = "bisonfoundin.txt";

    const long          BILLION = 1000000000L;
    uint64_t            elapsedTime;
    struct timespec     startTime, endTime;
    string              searchBisonInGrass;

    string input;
    string output;

    input = argv[1] + INPUT_FILE_NAME;
    output = argv[1] + OUTPUT_FILE_NAME;
    // input & output file both have the testcase number in front of filename

    ifstream infile;
    infile.open(input);
    ofstream outfile;
    outfile.open(output);

    if (infile.fail())
    {
        cout << "File " << INPUT_FILE_NAME << " could not be opened!" << endl;
        cout << endl << "Press the enter key once or twice to leave..." << endl;
        cin.ignore();
        cin.get();
        exit(EXIT_FAILURE);
    }
    // if file does not open, display error message

    infile >> searchBisonInGrass;
    // read string from input file to local variable

    unsigned    answerFoundBisonPattern = 0, backParenCount = 0;
    
    clock_gettime(CLOCK_MONOTONIC, &startTime);

    for (int i = 1, size = searchBisonInGrass.size(); i < size; i++)
    {
        if (searchBisonInGrass[i - 1] == ')' && searchBisonInGrass[i] == ')')
            answerFoundBisonPattern += backParenCount;
        // if 2 ')' found next to each other, number increases by counter of '('

        else if (searchBisonInGrass[i - 1] == '(' && searchBisonInGrass[i] == '(')
            backParenCount++;
        // if 2 '(' found next to each other, counter increases by 1
    }

    clock_gettime(CLOCK_MONOTONIC, &endTime);

    elapsedTime = BILLION * (endTime.tv_sec - startTime.tv_sec) + endTime.tv_nsec - startTime.tv_nsec;
    // time it took to find the number of distinct pairs

    cout << "Testcase " << argv[1] << " successfully finished." << endl;
    cout << "Results in " << output << endl; 
    cout << "elapsed time = " << (long long unsigned int) elapsedTime << " nanoseconds" << endl;

    outfile << answerFoundBisonPattern << endl;
    // prints number of patterns to output file

    outfile.close();
    return EXIT_SUCCESS;
}