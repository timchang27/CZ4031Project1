#include "disk.h"

using namespace std;

Disk::Disk(size_t diskSize, size_t blockSize)
{

    this->diskSize = diskSize;
    this->blockSize = blockSize;

    // allocate memory on the heap
    pMemAddress = new unsigned char[diskSize]();

    this->totalMemSizeUsed = 0;
    this->allocatedBlockCount = 0;

    this->rootBlockPtr = NULL;
    this->currentBlockPtr = NULL;

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

// bool Disk::allocateBlockStruct()
// {
//     // Allocate a new block and move free pointer to start of block.

//     if (blockIdx < maxBlocksPerDisk)
//     {
//         Block *newBlock = new Block();
//         newBlock->recordCount = 0;
//         newBlock->rootRecord = NULL;
//         newBlock->currentRecord = NULL;
//         newBlock->nextFreeRecord = NULL;
//         newBlock->nextBlock = NULL;

//         if (rootBlockPtr == NULL)
//         {
//             rootBlockPtr = newBlock;
//             currentBlockPtr = rootBlockPtr;
//         }
//         else
//         {
//             // currentBlockPtr = rootBlockPtr + (blockSize * allocatedBlockCount);
//             Block *currBlock = currentBlockPtr;
//             currBlock->nextBlock = newBlock;
//             currentBlockPtr = newBlock;
//         }

//         allocatedBlockCount++;
//         freeBlockCount--;
//         totalMemSizeUsed = totalMemSizeUsed + blockSize;
//         return true;
//     }
//     else
//     {
//         std::cout << "--- Error, out of memory for allocation ---\n";
//         std::cout << "Total Memory Size: " << diskSize << "\n";
//         std::cout << "Total Memory Size Used: " << totalMemSizeUsed << "\n";
//         return false;
//     }
// }

// struct Block *Disk::allocateRecordToMem(Record record)
// {

//     // Checking if the size of record exceeds the memory block size before allocation
//     if (sizeof(record) > blockSize)
//     {
//         std::cout << "--- Error, Record size exceeds memory block size ---\n";
//         std::cout << "Record Size: " << sizeof(record) << "\n";
//         std::cout << "Memory Block Size: " << blockSize << "\n";
//         return NULL;
//     }
//     else if ((allocatedBlockCount == 0 && freeBlockCount != 0) || (isCurrentBlockFull(sizeof(record))))
//     {
//         // no allocated blocks && there are still possible blocks to be allocated
//         // current block not full
//         bool success = allocateBlockStruct();

//         if (!success)
//         {
//             return NULL;
//         }
//     }

//     // Add record
//     if (currentBlockPtr->recordCount == 0)
//     {
//         // root record
//         currentBlockPtr->rootRecord = new struct Record();
//         memcpy(currentBlockPtr->rootRecord, &record, sizeof(record));
//         currentBlockPtr->currentRecord = currentBlockPtr->rootRecord;
//         currentBlockPtr->nextFreeRecord = currentBlockPtr->currentRecord->next;
//         (currentBlockPtr->recordCount)++;
//     }
//     else
//     {
//         // non root records
//         currentBlockPtr->nextFreeRecord = new struct Record();
//         memcpy(currentBlockPtr->nextFreeRecord, &record, sizeof(record));
//         currentBlockPtr->currentRecord->next = currentBlockPtr->nextFreeRecord;
//         currentBlockPtr->currentRecord = currentBlockPtr->nextFreeRecord;
//         currentBlockPtr->nextFreeRecord = currentBlockPtr->currentRecord->next;
//         (currentBlockPtr->recordCount)++;
//     }
//     return currentBlockPtr;
// }

// bool Disk::isCurrentBlockFull(std::size_t recordSize)
// {

//     if ((currentBlockPtr->recordCount * 20) >= ((int)blockSize))
//     {
//         // if (currentBlockPtr->recordCount==5){
//         return true;
//     }
//     else
//     {
//         return false;
//     }
// }

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