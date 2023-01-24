#include <iostream>
#include <string>
#include <fstream>
#include "storage/disk.cpp"

using namespace std;

int main()
{
    // Set block size to 200
    int BLOCKSIZE = 200;
    cout << "Hello World" << endl;

    // Handle File
    ifstream file("../data/data.tsv");
    if (file.is_open())
    {
        cout << "Open" << endl;
    }
    else
    {
        cout << "Not Open" << endl;
    }

    Disk *disk = new Disk(500);
    string line;
    int i = 0;
    while (getline(file, line))
    {
        if (i == 0)
        {
            i++;
            continue;
        }
        disk->insert(line); // add tuple to disk
    }
    file.close();
    // number of blocks output
    int numBlocks = disk->getTotalBlocks();
    cout << "Total Number of Blocks: " << numBlocks << endl;
    // the size of database (in terms of MB)
    int blocksize = disk->getBlockSizeinByte();
    // cout<<"the block size is "<<blocksize;
    float mb = (float(numBlocks * blocksize) / float(1024 * 1024));
    cout << "Size of database (in terms of MB): " << mb << endl;
    cout << "Program End" << endl;
}