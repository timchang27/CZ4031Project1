#include "tree.h"

Node *Tree::searchNode(int key)
{
}

std::vector<Record *> *Tree::searchRecord(int key)
{
    if (this->root == nullptr)
    {
        return nullptr;
    }

    Node *curNode = this->root;
    int idx = 0;

    while (!curNode->isLeaf)
    {
        idx = std::lower_bound(curNode->keys.begin(), curNode->keys.end(), key) - curNode->keys.begin();
        curNode = curNode->pointers.at(idx);
    }

    idx = std::lower_bound(curNode->keys.begin(), curNode->keys.end(), key) - curNode->keys.begin();
    if (idx < curNode->keys.size() && curNode->keys.at(idx) == key)
    {
        return &(curNode->records.at(idx));
    }

    return nullptr;
}