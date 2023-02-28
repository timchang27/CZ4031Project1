#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "storage/disk.h"
#include "tree/tree.h"

using namespace std;

int readFileIntoDisk(Disk *disk, Tree *tree)
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
        tree->insert(newRecord->numVotes, newRecord);
        numRecords++;
    }
    file.close();
    return numRecords;
}

void exp1(Disk *disk, Tree *tree)
{
    cout << "Experiment 1:" << endl;
    int numRecords = readFileIntoDisk(disk, tree);
    cout << "No of records: " << numRecords << endl;
    cout << "Size of record: " << sizeof(Record) << " bytes" << endl;
    cout << "No of records stored in a block: " << (*disk).getRecordsPerBlock() << endl;
    cout << "No of blocks used: " << (*disk).getBlocksUsed() << endl;
}

void exp2(Tree *t)
{
    cout << "Experiment 2:" << endl;
    std::cout << "Instantiating B+ Tree" << endl;
    // std::cout << " -> Nodes bounded by block size of = " << blockSize << endl;
    // std::cout << " -> Maximum number of keys in a node: n = " << this->maxKeys << endl;
    // std::cout << " -> Internal node max pointers to other nodes = " << this->maxKeys + 1 << endl;
    cout << "Parameter N = " << t->getMaxKeys() << endl;
    cout << "Number of levels = TBC" << endl;
    cout << "Content of Root Node:" << endl;
    t->displayKeys(t->getRoot());
}

int main()
{
    // Set block size to 200B and disk size to 100MB
    int BLOCKSIZE = 200;
    int DISKSIZE = 100 * pow(2, 20);
    Tree tree = Tree(BLOCKSIZE);

    Disk disk = Disk(DISKSIZE, BLOCKSIZE);
    exp1(&disk, &tree);

    exp2(&tree);
}
