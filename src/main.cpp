#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "storage/disk.cpp"

using namespace std;

int readFileIntoDisk(Disk *disk)
{
    ifstream file("../data/data.tsv");
    if (file.is_open())
    {
        cout << "File is open" << endl;
    }
    else
    {
        cout << "File failed to open" << endl;
        exit(EXIT_FAILURE);
    }
    Record *newRecord = nullptr;
    string line;
    int numRecords = 0;
    getline(file, line);
    while (getline(file, line))
    {
        istringstream iss(line);
        string tconst, averageRating, numVotes;
        getline(iss, tconst, '\t');
        getline(iss, averageRating, '\t');
        getline(iss, numVotes, '\t');
        newRecord = (*disk).insertRecord(tconst, (unsigned char)(stof(averageRating) * 10), stoi(numVotes));
        numRecords++;
    }
    file.close();
    return numRecords;
}

void exp1(Disk *disk)
{
    cout << "Experiment 1:" << endl;
    int numRecords = readFileIntoDisk(disk);
    cout << "No of records: " << numRecords << endl;
    cout << "Size of record: " << sizeof(Record) << " bytes" << endl;
    cout << "No of records stored in a block: " << (*disk).getRecordsPerBlock() << endl;
    cout << "No of blocks used: " << (*disk).getBlocksUsed() << endl;
}

int main()
{
    // Set block size to 200B and disk size to 100MB
    int BLOCKSIZE = 200;
    int DISKSIZE = 100 * pow(2, 20);

    Disk disk = Disk(DISKSIZE, BLOCKSIZE);
    exp1(&disk);
}
