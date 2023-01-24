#include "storage.h"
#include <iostream>

using namespace std;

Storage::Storage(size_t diskSize, size_t blockSize)
{
    this->diskSize = diskSize;
    this->blockSize = blockSize;
}