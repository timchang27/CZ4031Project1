#include "tree.h"
#include <algorithm>

void Tree::deleteKey(int key)
{
    /**
     * Case 0: Key does not exist in the tree
     * Case 1: Leaf node has > minimum num of keys
     *      1a: key is not the left most key, just delete
     *      1b: key is the left most, check if parent nodes require updating with new left most key
     * Case 2: Leaf node has min num of keys
     *      2a: Check neighbour node that is from same parent, borrow if neighbour meets min key req
     *      2b: If same parent neighbour does not meet min key requirement, borrow from other neighbour
     *      2c: If both don't meet req, merge with left neighbour (?)
     *
     *      2a & 2b should prioritise left neighbour if both are from same parent
     */

    if (this->root == nullptr)
    {
        std::cout << "Tree is empty!" << std::endl;
        return;
    }

    std::vector<Record *> *r = this->searchRecord(key);
    if (r == nullptr)
    {
        // Case 0: Key does not exist in the tree
        std::cout << "Key " << key << " does not exist in the B+ Tree!" << std::endl;
        return;
    }

    Node *curNode = this->root;
    std::vector<Node *> parents(1, nullptr);
    std::vector<int> prevIdxs;
    short idx, minKeys = (this->maxKeys + 1) / 2;

    // Find the leaf node where the key belongs, saving the internal nodes visited and indexes used
    while (!curNode->isLeaf)
    {
        idx = std::upper_bound(curNode->keys.begin(), curNode->keys.end(), key) - curNode->keys.begin();
        parents.push_back(curNode);
        prevIdxs.push_back(idx);
        curNode = curNode->pointers.at(idx);
    }

    // Find the key's index in the leaf node, and delete the key and its records
    idx = std::lower_bound(curNode->keys.begin(), curNode->keys.end(), key) - curNode->keys.begin();
    curNode->keys.erase(curNode->keys.begin() + idx);
    curNode->records.erase(curNode->records.begin() + idx);

    if (curNode == this->root && curNode->keys.size() == 0)
    {
        // Check if the there are no more keys in the tree, in which case set root to null
        this->setRoot(nullptr);
    }

    if (curNode->keys.size() >= minKeys)
    {
        // Case 1: Leaf node has > minimum num of keys
        if (idx != 0)
        {
            // Case 1a: key is not the left most key, just delete
            return;
        }

        while (parents.back() != nullptr)
        {
            // Case 1b: key is the left most in the leaf node
            if (prevIdxs.back() == 0)
            {
                // If parent's idx was 0 also, iteratively check its parent
                parents.pop_back();
                prevIdxs.pop_back();
            }
            else
            {
                // If parent's idx was not 0, update the key with the new left most key of the leaf node
                key = curNode->keys.front();
                curNode = parents.back();
                idx = prevIdxs.back();
                curNode->keys[idx - 1] = key;
                return;
            }
        }
    }
    else
    {
        // Case 2: Leaf node now has less than min num of keys
        int parentIdx = prevIdxs.back();
        prevIdxs.pop_back();
        Node *leftNeighbour = nullptr, *rightNeighbour = nullptr;
        Node *parentNode = parents.back();
        parents.pop_back();

        if (parentIdx > 0)
        {
            // Left neighbour from the same parent node exists
            leftNeighbour = parentNode->pointers.at(parentIdx - 1);
            if (leftNeighbour->keys.size() > minKeys)
            {
                // Case 2a: Borrow key from left neighbour
                curNode->keys.insert(curNode->keys.begin(), leftNeighbour->keys.back());
                curNode->records.insert(curNode->records.begin(), leftNeighbour->records.back());
                leftNeighbour->keys.pop_back();
                leftNeighbour->records.pop_back();
                parentNode->keys[parentIdx] = curNode->keys.front();
                return;
            }
        }
        if (parentIdx < parentNode->keys.size() - 1)
        {
            // Right neighbour from the same parent node exists
            rightNeighbour = parentNode->pointers.at(parentIdx + 1);
            if (rightNeighbour->keys.size() > minKeys)
            {
                // Case 2a: Borrow key from right neighbour
                curNode->keys.insert(curNode->keys.end(), rightNeighbour->keys.front());
                curNode->records.insert(curNode->records.end(), rightNeighbour->records.front());
                rightNeighbour->keys.erase(rightNeighbour->keys.begin());
                rightNeighbour->records.erase(rightNeighbour->records.begin());
                parentNode->keys[parentIdx] = rightNeighbour->keys.front();

                if (!idx)
                {
                    this->updateParentKeys(curNode, parentNode, parentIdx, parents, prevIdxs);
                }
                return;
            }
        }

        // Check if left neighbour exists, if it does merge with it, else merge with right neighbour
        if (leftNeighbour != nullptr)
        {
            // Left neighbour exists but has min keys -> MERGE
            while (curNode->keys.size() != 0)
            {
                leftNeighbour->keys.push_back(curNode->keys.front());
                leftNeighbour->records.push_back(curNode->records.front());
                curNode->keys.erase(curNode->keys.begin());
                curNode->records.erase(curNode->records.begin());
            }
            parentNode->keys.erase(parentNode->keys.begin() + parentIdx - 1);
            parentNode->pointers.erase(parentNode->pointers.begin() + parentIdx);
            leftNeighbour->nxtLeaf = curNode->nxtLeaf;
        }
        else
        {
            // Right neighbour exists but has min keys -> MERGE
            while (rightNeighbour->keys.size() != 0)
            {
                curNode->keys.push_back(rightNeighbour->keys.front());
                curNode->records.push_back(rightNeighbour->records.front());
                rightNeighbour->keys.erase(rightNeighbour->keys.begin());
                rightNeighbour->records.erase(rightNeighbour->records.begin());
            }
            parentNode->keys.erase(parentNode->keys.begin() + parentIdx);
            parentNode->pointers.erase(parentNode->pointers.begin() + parentIdx + 1);
            curNode->nxtLeaf = rightNeighbour->nxtLeaf;

            if (!idx)
            {
                this->updateParentKeys(curNode, parentNode, parentIdx, parents, prevIdxs);
            }
        }

        // After merging, check if parent node has min num of keys
        if (parentNode->keys.size() < this->maxKeys / 2)
        {
            // Parent has less than min keys required for internal nodes
            // Borrow or merge?
        }
    }
}

void Tree::removeInternal(int key, Node *currentNode, Node *child)
{
}

void Tree::updateParentKeys(Node *curNode, Node *parentNode, int parentIdx, std::vector<Node *> &parents, std::vector<int> &prevIdxs)
{
    while (parentNode != nullptr)
    {
        // Iteratively check and update the parent nodes
        if (parentIdx == 0)
        {
            // If parent's idx was 0 also, iteratively check its parent
            parentNode = parents.back();
            parents.pop_back();
            parentIdx = prevIdxs.back();
            prevIdxs.pop_back();
        }
        else
        {
            // If parent's idx was not 0, update the key with the new left most key of the leaf node
            int key = curNode->keys.front();
            parentNode->keys[parentIdx - 1] = key;
            break;
        }
    }
}