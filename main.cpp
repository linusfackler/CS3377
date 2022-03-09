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

    infile >> searchBisonInGrass;

    unsigned    answerFoundBisonPattern = 0,
                backParenCount         = 0;
    
    clock_gettime(CLOCK_MONOTONIC, &startTime);

    for (int i = 1, size = searchBisonInGrass.size(); i < size; i++)
    {
        if (searchBisonInGrass[i - 1] == ')' && searchBisonInGrass[i] == ')')
            answerFoundBisonPattern += backParenCount;

        else if (searchBisonInGrass[i - 1] == '(' && searchBisonInGrass[i] == '(')
            backParenCount++;
    }

    clock_gettime(CLOCK_MONOTONIC, &endTime);

    elapsedTime = BILLION * (endTime.tv_sec - startTime.tv_sec) + endTime.tv_nsec - startTime.tv_nsec;

    cout << "Testcase " << argv[1] << " successfully finished." << endl;
    cout << "Results in " << output << endl; 
    cout << "elapsed time = " << (long long unsigned int) elapsedTime << " nanoseconds" << endl;

    outfile << answerFoundBisonPattern << endl;

    outfile.close();

    

    return EXIT_SUCCESS;
}