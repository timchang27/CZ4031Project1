#ifndef DISK_H
#define DISK_H

#include <cstddef>
#include <string>
#include <iostream>
#include <cmath>
#include <cstring>

struct Record
{
    char tconst[11];
    unsigned char averageRating;
    int numVotes;
};

class Disk
{
private:
    size_t blockSize;
    size_t diskSize;
    size_t blockIdx;
    size_t recordIdx;
    unsigned char *pDiskAddress;

    size_t maxRecordsPerBlock;
    size_t maxBlocksPerDisk;
    size_t totalMemSizeUsed;

public:
    // constructor
    Disk(size_t aDiskSize, size_t aBlockSize);

    // functions
    Record *insertRecord(const std::string &tconst, unsigned char avgRating, int numVotes);

    Record *getRecord(size_t aBlockIdx, size_t aRecordIdx);

    void printRecord(Record *record);

    size_t getBlockId(Record *record);

    void printBlock(size_t aBlockIdx);

    size_t getBlocksUsed();

    size_t getRecordsPerBlock();

    size_t getTotalUsedMemory();
};

#endif