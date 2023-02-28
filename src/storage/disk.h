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
    // struct Record *next;
    // Record()
    // {
    //     next = NULL;
    // }
};

// struct Block
// {
//     int recordCount;
//     struct Record *rootRecord;     // root record node
//     struct Record *currentRecord;  // current record node
//     struct Record *nextFreeRecord; // memcpy into this

//     struct Block *nextFreeBlock;
//     struct Block *nextBlock;
// };

class Disk
{
private:
    size_t blockSize;
    size_t diskSize;
    size_t blockIdx;
    size_t recordIdx;
    unsigned char *pMemAddress;

    size_t maxRecordsPerBlock;
    size_t maxBlocksPerDisk;
    size_t totalMemSizeUsed;

    int allocatedBlockCount;
    int usedBlockCount;
    int freeBlockCount;

    struct Block *rootBlockPtr;
    struct Block *currentBlockPtr;
    struct Block *nextFreeBlockPtr;

public:
    // constructor
    Disk(size_t aDiskSize, size_t aBlockSize);

    // functions
    // bool allocateBlockStruct();

    // struct Block *allocateRecordToMem(Record record);

    // bool isCurrentBlockFull(std::size_t recordSize);

    Record *insertRecord(const std::string &tconst, unsigned char avgRating, int numVotes);

    Record *getRecord(size_t aBlockIdx, size_t aRecordIdx);

    void printRecord(Record *record);

    size_t getBlockId(Record *record);

    void printBlock(size_t aBlockIdx);

    size_t getBlocksUsed();

    size_t getRecordsPerBlock();
};

#endif