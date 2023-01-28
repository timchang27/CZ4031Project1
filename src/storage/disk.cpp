#include "disk.h"

#include <iostream>
#include <cmath>
#include <cstring>

using namespace std;

Disk::Disk(size_t diskSize, size_t blockSize)
{

    this->diskSize = diskSize;
    this->blockSize = blockSize;

    // allocate memory on the heap
    pMemAddress = new unsigned char[diskSize]();

    // calculate maxes
    maxRecordsPerBlock = floor(blockSize / sizeof(Record));
    maxBlocksPerDisk = floor(diskSize / blockSize);

    // initialize indexes to 0
    blockIdx = 0;
    recordIdx = 0;
    cout << "Instantiating Disk" << endl;
    cout << " -> Disk Size: " << diskSize / pow(2, 20) << " MB" << endl;
    cout << " -> Block Size: " << blockSize << " bytes" << endl;
    cout << " -> Max Records Per Block: " << maxRecordsPerBlock << endl;
    cout << " -> Max Blocks in Disk: " << maxBlocksPerDisk << endl;
    cout << "===========================================" << endl;
}

Record *Disk::insertRecord(const string &tconst, unsigned char avgRating, int numVotes)
{
    // if disk is full, return nullptr
    if (blockIdx >= maxBlocksPerDisk)
    {
        return nullptr;
    }

    // get pointer to the new record
    Record *newRecord = getRecord(blockIdx, recordIdx);

    // set values into the  new record
    strncpy(newRecord->tconst, tconst.c_str(), sizeof(newRecord->tconst) - 1);
    newRecord->numVotes = numVotes;
    newRecord->averageRating = avgRating;

    // increment recordIdx
    recordIdx++;

    // if the currentNode block is full after this insertion, move to next block and reset recordIdx to 0
    if (recordIdx == maxRecordsPerBlock)
    {
        blockIdx++;
        recordIdx = 0;
    }

    // return a pointer to the inserted record
    return newRecord;
}

Record *Disk::getRecord(size_t blockIdx, size_t recordIdx)
{
    size_t offset = (blockIdx * blockSize) + (recordIdx * sizeof(Record));
    return reinterpret_cast<Record *>(pMemAddress + offset);
}

void Disk::printRecord(Record *record)
{
    printf("Record ID: %s \nAverage Rating: %.1f \n Number of Votes: %d\n", record->tconst, (float)record->averageRating / 10, record->numVotes);
}

size_t Disk::getBlockId(Record *record)
{
    return (reinterpret_cast<unsigned char *>(record) - pMemAddress) / blockSize;
}

void Disk::printBlock(size_t aBlockIdx)
{
    cout << "Contents of Data Block (blockIdx=" << aBlockIdx << "):" << endl;

    for (int i = 0; i < maxRecordsPerBlock; i++)
    {
        cout << getRecord(aBlockIdx, i)->tconst << " ";
    }
    cout << endl;
}

size_t Disk::getBlocksUsed()
{
    return blockIdx + 1;
}

size_t Disk::getRecordsPerBlock()
{
    return maxRecordsPerBlock;
}