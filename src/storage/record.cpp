#include <iostream>
#include <string>
#include <sstream>
#include <iterator>
#include <vector>
#include <string.h>

using namespace std;
class Record
{
public:
    char tconst[11];
    float averageRating;
    int numVotes;

    Record(string s)
    {
        vector<string> tokens;
        istringstream iss(s);
        copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter(tokens));
        strcpy(tconst, tokens[0].c_str());
        averageRating = stof(tokens[1]);
        numVotes = stoi(tokens[2]);
    }

    string getTconst()
    {
        return this->tconst;
    }

    double getAverageRating()
    {
        return this->averageRating;
    }

    int getNumVotes()
    {
        return this->numVotes;
    }

    string toString()
    {
        ostringstream out;
        out << this->tconst << "\t" << this->averageRating << "\t" << this->numVotes << "\n";
        return out.str();
    }
};