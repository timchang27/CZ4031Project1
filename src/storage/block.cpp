#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <utility>
#include "record.cpp"
using namespace std;

class Block
{
public:
    int numberSlot;
    int lastPosition;
    int size;
    char *m;

    Block(int x) : m(new char[x - 2])
    {
        size = x;
        numberSlot = 0;
        lastPosition = x - 2;
    }

    int add(Record record)
    {
        int temp1 = lastPosition - (int)sizeof(record);
        int temp2 = numberSlot * sizeof(int);

        if (temp1 < temp2)
        {
            return -1;
        }
        int newSlotId = 0;
        while (newSlotId < numberSlot && *((int *)(m + 4 * newSlotId)) != 0)
        {
            newSlotId += 1;
        }
        memcpy(m + lastPosition - sizeof(record), &record, sizeof(record));
        lastPosition -= sizeof(record);
        memcpy(m + 4 * newSlotId, &lastPosition, sizeof(lastPosition));
        if (!(newSlotId < numberSlot))
            numberSlot += 1;
        return newSlotId;
    }

    Record getRecord(int slotId)
    {
        int lastPosition = *((int *)(m + 4 * slotId));
        return *(Record *)(m + lastPosition);
    }

    void deleteSlot(int slotId)
    {
        int oldLastposition = *((int *)(m + 4 * slotId));

        memmove(m + lastPosition + sizeof(Record), m + lastPosition, oldLastposition - lastPosition);
        lastPosition += sizeof(Record);
        for (int i = slotId + 1; i < numberSlot; i++)
        {
            int tempPosition = *((int *)(m + 4 * i));
            if (tempPosition != 0)
                *((int *)(m + 4 * i)) = tempPosition + sizeof(Record);
        }
        *((int *)(m + 4 * slotId)) = 0;
    }

    void print()
    {
        for (int i = 0; i < numberSlot; i++)
        {
            int temp = *((int *)(m + 4 * i));
            cout << temp << "|";
        }
        for (int i = numberSlot; i < lastPosition; i++)
        {
            cout << ".";
        }
        cout << "\n";
        cout << "tconst\taverageRating\tnumVotes" << endl;
        for (int i = lastPosition; i + sizeof(Record) <= size - 2; i += (sizeof(Record)))
        {
            Record temp = *(Record *)(m + i);
            cout << temp.toString();
        }
    }
};
