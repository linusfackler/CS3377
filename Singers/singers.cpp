#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <vector>
#include <unistd.h>
#include <pthread.h>

using namespace std;

pthread_mutex_t lock;

static unsigned totalLineCount;

void getLyricLinesFromFileToVector (string lyricsFilenameStr, vector<string>& lyricLinesVector, unsigned& noOfLyricLines)
{
    ifstream fileLyricsStream(lyricsFilenameStr);
    if (fileLyricsStream.fail()) {
        cerr << "File " << lyricsFilenameStr << " could not be opened!" << endl;
        exit(EXIT_FAILURE);
    }

    string line;
    while (getline(fileLyricsStream, line)) {
        lyricLinesVector.push_back(line);
    }

    noOfLyricLines = lyricLinesVector.size();
}

struct SingLinesThreadInfoStruct
{
    unsigned* singerNoIdPtr;
    vector<string> lyricLinesVector;
    unsigned noOfLyricLines;
};

void* SingLinesThread (void* singLinesThreadInfoStructPtr)
{
    SingLinesThreadInfoStruct* threadInfoPtr = (SingLinesThreadInfoStruct*)singLinesThreadInfoStructPtr;

    unsigned* singersNoIdPtr = threadInfoPtr->singerNoIdPtr;
    vector<string> lyricLinesVector = threadInfoPtr->lyricLinesVector;
    unsigned noOfLyricLines = threadInfoPtr->noOfLyricLines;

    pthread_mutex_lock(&lock);
    cout << "start thread " << *singersNoIdPtr << endl;
    pthread_mutex_unlock(&lock);

    for (unsigned lyricLineNo = 0; lyricLineNo < noOfLyricLines; lyricLineNo++) {
        pthread_mutex_lock(&lock);
        cout << setw(3) << *singersNoIdPtr << ": " << setw(3) << lyricLineNo + 1 <<  " > " << lyricLinesVector[lyricLineNo] << endl;
        ++totalLineCount;
        pthread_mutex_unlock(&lock);
    }
    
    cout << "done thread " << *singersNoIdPtr << endl;

    pthread_exit(NULL);
}

int main (int argc, char* argv[])
{
    if (argc != 3) {
        cerr << endl << "usage: " << endl << endl;
        cerr << endl << "singers <lyrics filename.txt> <number of singers>" << endl << endl;
        cerr << endl << "Example: " << endl << endl;
        cerr << endl << "singers PopLife.txt 4 " << endl << endl << endl;
    }

    string lyricsFilenameStr(argv[1]);
    unsigned noOfSingers = stoi(argv[2]);
    vector<string> lyricLinesVector;
    unsigned noOfLyricLines = 0;

    getLyricLinesFromFileToVector(lyricsFilenameStr, lyricLinesVector, noOfLyricLines);

    cout << endl << endl;
    cout << "Song lyrics File Name is: " << lyricsFilenameStr << endl;
    cout << "Number of lyric lines is: " << noOfLyricLines << endl;
    cout << endl << endl;

    pthread_t* singersThreadIdPtr;
    unsigned* singersNoIdPtr;
    SingLinesThreadInfoStruct* singLinesThreadInfoStructPtr;
    
    singersThreadIdPtr = new pthread_t[noOfSingers];
    singersNoIdPtr = new unsigned [noOfSingers];
    singLinesThreadInfoStructPtr = new SingLinesThreadInfoStruct[noOfSingers];

    if (pthread_mutex_init(&lock, NULL) != 0) {
        cerr << "mutex init failed " << endl;
        exit(EXIT_FAILURE);
    }

    for (unsigned singerNo = 0; singerNo < noOfSingers; ++singerNo)
    {
        unsigned *singerNoIdPtr = new unsigned;
        *singerNoIdPtr = singerNo + 1;

        singLinesThreadInfoStructPtr[singerNo].singerNoIdPtr = singerNoIdPtr;
        singLinesThreadInfoStructPtr[singerNo].lyricLinesVector = lyricLinesVector;
        singLinesThreadInfoStructPtr[singerNo].noOfLyricLines = noOfLyricLines;

        pthread_mutex_lock(&lock);
        cout << "create thread " << *singerNoIdPtr << endl;
        pthread_mutex_unlock(&lock);

        int threadCreateReturn = pthread_create(&(singersThreadIdPtr[singerNo]), NULL, SingLinesThread, (void *)&singLinesThreadInfoStructPtr[singerNo]);
        if (threadCreateReturn != 0) {
            cerr << "create thread failed " << singerNo << endl;
            exit(EXIT_FAILURE);
        }
    }

    for (int singerNo = 0; singerNo < noOfSingers; ++singerNo)
    {
        pthread_join(singersThreadIdPtr[singerNo], NULL);
    }

    cout << endl;
    cout << "Program done " << endl;
    cout << "Total line count sung: " << totalLineCount << endl;
    cout << endl;
    pthread_mutex_destroy(&lock);

    exit(EXIT_SUCCESS);
}
