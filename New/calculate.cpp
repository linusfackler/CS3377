//more
#include <vector>
#include <algorithm>
#include <iterator>

using namespace std;

unsigned vectorsum(vector<int> vectorInts)
{
    int sum = 0;
    for (auto it = vectorInts.begin(); it = vectorInts.end(); it++)
        sum+=*it;

    return (sum);
}

float vectoraverage(vector<int> vectorInts)
{

}

unsigned vectorleast(vector<int> vectorInts)
{
    int least = vectorInts[0];

    for (auto it = vectorInts.begin(); it != vectorInts.end(); it++)
        if (least > *it) least = *it;
    
    return(least);
}

unsigned vectorgreatest(vector<int> vectorInts)
{

}

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        cout << endl << endl <<
            "Wrong arguments" << endl
                "Usage: " << endl <<
                    "calculate < sum | average | greatest | least > filenae.txt" << endl <<
                        "Example:" << endl << 
                            "calculate sum filerandom.txt" << endl << endl;
        exit(EXIT_FAILURE)
    }

    cerr << endl << argv[0] << " " << argv[1] << " " << argv[2] << endl;

    vector<int> vectorInts;

    ifstream randomNos(argv[2]);
    istream_iterator<int> inputIt(randomNos);

    copy(inputIt, istream_iterator<int>(), back_inserter(vectorInts));

    string command = srgv[1];

    if (command == "sum")
        cout << "The sum is " << vectorsum(vectorInts) << endl;   
    else if (command == "average")
        cout << "The average is " << vectoraverage(vectorInts) << endl;
    else if (command == "least")
        cout << "The least is " << vectorleast(vectorInts) << endl;
    else if (command == "greatest")
        cout << "The greatest is " << vectorgreatest(vectorInts) << endl;
    else
        cerr << "Invalid Command" << endl;

    cout << endl << endl;

    return(EXIT_SUCCESS);
}