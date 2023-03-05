#include "disk.h"

using namespace std;

Disk::Disk(size_t diskSize, size_t blockSize)
{

    this->diskSize = diskSize;
    this->blockSize = blockSize;

    pDiskAddress = new unsigned char[diskSize]();

    this->totalMemSizeUsed = 0;

    maxRecordsPerBlock = floor(blockSize / sizeof(Record));
    maxBlocksPerDisk = floor(diskSize / blockSize);

    blockIdx = 0;
    recordIdx = 0;
    cout << "Instantiating Disk" << endl;
    cout << "Disk Size: " << diskSize / pow(2, 20) << " MB" << endl;
    cout << "Block Size: " << blockSize << " bytes" << endl;
    cout << "Max Records Per Block: " << maxRecordsPerBlock << endl;
    cout << "Max Blocks in Disk: " << maxBlocksPerDisk << endl;
    cout << "===========================================" << endl;
}

Record *Disk::insertRecord(const string &tconst, unsigned char avgRating, int numVotes)
{
    if (blockIdx >= maxBlocksPerDisk)
    {
        return nullptr;
    }

    // point to new record
    Record *newRecord = getRecord(blockIdx, recordIdx);

    // copy data values into new record
    strncpy(newRecord->tconst, tconst.c_str(), sizeof(newRecord->tconst) - 1);
    newRecord->numVotes = numVotes;
    newRecord->averageRating = avgRating;

    if (blockIdx == 0 && recordIdx == 0)
    {
        totalMemSizeUsed = totalMemSizeUsed + blockSize;
    }

    recordIdx++;

    if (recordIdx == maxRecordsPerBlock)
    {
        blockIdx++;
        totalMemSizeUsed = totalMemSizeUsed + blockSize;
        recordIdx = 0;
    }

    return newRecord;
}

Record *Disk::getRecord(size_t blockIdx, size_t recordIdx)
{
    size_t offset = (blockIdx * blockSize) + (recordIdx * sizeof(Record));
    return reinterpret_cast<Record *>(pDiskAddress + offset);
}

void Disk::printRecord(Record *record)
{
    printf("Record ID: %s \nAverage Rating: %.1f \n Number of Votes: %d\n", record->tconst, (float)record->averageRating / 10, record->numVotes);
}

size_t Disk::getBlockId(Record *record)
{
    return (reinterpret_cast<unsigned char *>(record) - pDiskAddress) / blockSize;
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

size_t Disk::getTotalUsedMemory()
{
    return totalMemSizeUsed;
}