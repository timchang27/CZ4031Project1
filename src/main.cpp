#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <chrono>
#include <unordered_set>
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
    cout << endl;
}

void exp2(Tree *tree)
{
    cout << "Experiment 2:" << endl;
    cout << "Instantiating B+ Tree" << endl;
    cout << " -> Nodes bounded by block size of = " << tree->getBlockSize() << endl;
    cout << " -> Maximum number of keys in a node: n = " << tree->getMaxKeys() << endl;
    // cout << " -> Internal node max pointers to other nodes = " << t->getMaxKeys() + 1 << endl;
    cout << "Parameter N = " << tree->getMaxKeys() << endl;
    cout << "Number of nodes = " << tree->getTotalNumOfNodes() << endl;
    cout << "Number of levels = " << tree->getDepth() << endl;
    cout << "Content of Root Node: ";
    tree->displayKeys(tree->getRoot());
    cout << endl;
}

void exp3(Disk *disk, Tree *tree){
    tree->setNumOfNodesAcc(0);
    chrono::high_resolution_clock::time_point before = chrono::high_resolution_clock::now();
    vector<Record*> *result = tree->searchRecord(500);
    chrono::high_resolution_clock::time_point after = chrono::high_resolution_clock::now();
    chrono::duration<double> timeTaken = chrono::duration_cast< chrono::duration<double> >(after - before);

    unordered_set<size_t> resultSet;
    float totalRating = 0;
    for (Record *r : *result){
        resultSet.insert(disk->getBlockId(r));
        totalRating += r->averageRating;
        // cout << "Rating = " << r->averageRating << endl;
    }
    totalRating /= 10;

    int numOfBlocksAcc = 0;
    Record *r;
    before = chrono::high_resolution_clock::now();
    for (int i = 0; i < disk->getBlocksUsed(); i++){
        numOfBlocksAcc++;
        for (int j = 0; j < disk->getRecordsPerBlock(); j++){
            r = disk->getRecord(i, j);
            if (r->numVotes == 500){
                continue;
            }
        }
    }
    after = chrono::high_resolution_clock::now();
    chrono::duration<double> bruteTimeTaken = chrono::duration_cast< chrono::duration<double> >(after - before);

    cout << "Experiment 3:" << endl;
    cout << "Number of index blocks accessed = " << tree->getNumOfNodesAcc() << endl;
    cout << "Number of data blocks accessed = " << resultSet.size() << endl;
    cout << "Average rating = " << totalRating/result->size() << endl; 
    cout << "Running time for retrieval process = " << timeTaken.count() << "s" << endl;
    cout << "Number of data blocks accessed by brute force method = " << numOfBlocksAcc << endl;
    cout << "Running time for retrival by brute force method = " << bruteTimeTaken.count() << "s" << endl;
    cout << endl;
}

void exp4(Disk *disk, Tree *tree){
    vector<Record*> result;
    vector<int> keys;
    unsigned short lower = 30000;
    unsigned short upper = 40000;
    int lowerIdx, upperIdx, leafNodesAcc = 0;
    bool searching = true;
    tree->setNumOfNodesAcc(0);

    chrono::high_resolution_clock::time_point before = chrono::high_resolution_clock::now();
    Node *resultNode = tree->searchNode(lower);
    while (searching){
        // tree->displayKeys(resultNode);
        keys = resultNode->getKeys();
        lowerIdx = lower_bound(keys.begin(), keys.end(), lower) - keys.begin();
        upperIdx = lower_bound(keys.begin(), keys.end(), upper) - keys.begin();
        for (int i = lowerIdx; i < upperIdx; i++){
            // cout<< "lowerIdx = " << lowerIdx << " upperIdx = " << upperIdx << " resultRecord pointers = " << resultNode->getRecords().size() << endl;
            for (Record *r : resultNode->getRecords(i)){
                result.push_back(r);
            }
        }
        upperIdx = upperIdx == keys.size() ? upperIdx - 1 : upperIdx;
        if (keys.at(upperIdx) >= upper){
            searching = false;
        }
        else{
            resultNode = resultNode->getNxtLeaf();
            leafNodesAcc++;
        }
    }
    chrono::high_resolution_clock::time_point after = chrono::high_resolution_clock::now();
    chrono::duration<double> timeTaken = chrono::duration_cast< chrono::duration<double> >(after - before);

    unordered_set<size_t> resultSet;
    float totalRating = 0;
    for (Record *r : result){
        resultSet.insert(disk->getBlockId(r));
        totalRating += r->averageRating;
        // cout << "Rating = " << r->averageRating << endl;
    }
    totalRating /= 10;

    int numOfBlocksAcc = 0;
    Record *r;
    before = chrono::high_resolution_clock::now();
    for (int i = 0; i < disk->getBlocksUsed(); i++){
        numOfBlocksAcc++;
        for (int j = 0; j < disk->getRecordsPerBlock(); j++){
            r = disk->getRecord(i, j);
            if (r->numVotes >= lower && r->numVotes <= upper){
                continue;
            }
        }
    }
    after = chrono::high_resolution_clock::now();
    chrono::duration<double> bruteTimeTaken = chrono::duration_cast< chrono::duration<double> >(after - before);

    cout << "Experiment 4:" << endl;
    cout << "Number of index blocks accessed = " << tree->getNumOfNodesAcc() + leafNodesAcc << endl;
    cout << "Number of data blocks accessed = " << resultSet.size() << endl;
    cout << "Average rating = " << totalRating / result.size() << endl;
    cout << "Running time for retrieval process = " << timeTaken.count() << "s" << endl;
    cout << "Number of data blocks accessed by brute force method = " << numOfBlocksAcc << endl;
    cout << "Running time for retrival by brute force method = " << bruteTimeTaken.count() << "s" << endl;
    cout << endl;
}

void exp5(Disk *disk, Tree *tree){
    int keyToDelete = 1000;
    chrono::high_resolution_clock::time_point before = chrono::high_resolution_clock::now();
    tree->deleteKey(1000);
    chrono::high_resolution_clock::time_point after = chrono::high_resolution_clock::now();
    chrono::duration<double> timeTaken = chrono::duration_cast< chrono::duration<double> >(after - before);

    int numOfBlocksAcc = 0;
    Record *r;
    before = chrono::high_resolution_clock::now();
    for (int i = 0; i < disk->getBlocksUsed(); i++){
        numOfBlocksAcc++;
        for (int j = 0; j < disk->getRecordsPerBlock(); j++)
        {
            r = disk->getRecord(i, j);
            if (r->numVotes == keyToDelete){
                continue;
            }
        }
    }
    after = chrono::high_resolution_clock::now();
    chrono::duration<double> bruteTimeTaken = chrono::duration_cast< chrono::duration<double> >(after - before);

    cout << "Experiment 5:" << endl;
    cout << "Number of nodes of the updated B+ Tree = " << tree->getTotalNumOfNodes() << endl;
    cout << "Number of levels of the updated B+ Tree = " << tree->getDepth() << endl;
    cout << "Content of Root Node of updated B+ Tree: ";
    tree->displayKeys(tree->getRoot());
    cout << "Running time for deletion process = " << timeTaken.count() << "s" << endl;
    cout << "Number of data blocks accessed by brute force method = " << numOfBlocksAcc << endl;
    cout << "Running time for deletion by brute force method = " << bruteTimeTaken.count() << "s" << endl;
    cout << endl;
}

int main()
{
    // Set block size to 200B and disk size to 100MB
    int BLOCKSIZE = 200;
    int DISKSIZE = 100 * pow(2, 20);
    Disk disk(DISKSIZE, BLOCKSIZE);
    Tree tree(BLOCKSIZE);

    exp1(&disk, &tree);

    exp2(&tree);
    
    exp3(&disk, &tree);

    exp4(&disk, &tree);

    exp5(&disk, &tree);
}
