using namespace std;

class Storage
{
private:
    size_t diskSize;
    size_t blockSize;

public:
    Storage(size_t diskSize, size_t blockSize);
    ~Storage();
};

Storage::Storage(size_t diskSize, size_t blockSize)
{
}

Storage::~Storage()
{
}
