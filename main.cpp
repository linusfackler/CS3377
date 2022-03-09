#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

using namespace std;

const char *INPUT_FILE_NAME = "01bisonsearchin.txt";
const char *OUTPUT_FILE_NAME = "bisonfoundin.txt";

int main()
{
    cout.imbue(locale(""));

    const long          BILLION = 1000000000L;
    uint64_t            elapsedTime;
    struct timespec     startTime, endTime;
    string              searchBisonInGrass;

    ifstream infile (INPUT_FILE_NAME);
    ofstream outfile (OUTPUT_FILE_NAME);

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

    for (unsigned i = 1, size = searchBisonInGrass.size(); i < size; i++)
    {
        if (searchBisonInGrass[i - 1] == ')' && searchBisonInGrass[i] == 'i')
            answerFoundBisonPattern += backParenCount;

        else if (searchBisonInGrass[i - 1] == '(' && searchBisonInGrass[i] == '(')
            backParenCount++;
    }

    clock_gettime(CLOCK_MONOTONIC, &endTime);

    elapsedTime = BILLION * (endTime.tv_sec - startTime.tv_sec) + endTime.tv_nsec - startTime.tv_nsec;

    cout << "elapsed time = " << (long long unsigned int) elapsedTime << " nanoseconds" << endl;

    outfile << answerFoundBisonPattern << endl;

    outfile.close();

    return EXIT_SUCCESS;
}