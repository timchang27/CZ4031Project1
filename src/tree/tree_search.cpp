#include "tree.h"
#include <algorithm>

<<<<<<< HEAD
Node *Tree::searchNode(int key)
{
=======
Node* Tree::searchNode(int key){
    if (this->root == nullptr){
        return nullptr;
    }

    Node *curNode = this->root;
    int idx;
    this->numOfNodesAcc++;

    while (!curNode->isLeaf){
        idx = std::upper_bound(curNode->keys.begin(), curNode->keys.end(), key) - curNode->keys.begin();
        curNode = curNode->pointers.at(idx);
        this->numOfNodesAcc++;
    }

    return curNode;
>>>>>>> f8b18e9b8387d0b50889561a179f2ad71ddf7f3d
}

std::vector<Record *> *Tree::searchRecord(int key)
{
    if (this->root == nullptr)
    {
        return nullptr;
    }

    Node *curNode = this->root;
    int idx;
    this->numOfNodesAcc++;

    while (!curNode->isLeaf)
    {
        idx = std::upper_bound(curNode->keys.begin(), curNode->keys.end(), key) - curNode->keys.begin();
        curNode = curNode->pointers.at(idx);
        this->numOfNodesAcc++;
    }

    idx = std::lower_bound(curNode->keys.begin(), curNode->keys.end(), key) - curNode->keys.begin();
    if (idx < curNode->keys.size() && curNode->keys.at(idx) == key)
    {
        return &(curNode->records.at(idx));
    }

    return nullptr;
}