#include "tree.h"

void Tree::deleteKey(int key){
    /**
     * Case 0: Key does not exist in the tree
     * Case 1: Leaf node has > minimum num of keys
     *      1a: key is not the left most key, just delete
     *      1b: key is the left most, check if parent nodes require updating with new left most key
     * Case 2: Leaf node has == min num of keys
     *      2a: Check neighbour node that is from same parent, borrow if neighbour meets min key req
     *      2b: If same parent neighbour does not meet min key requirement, borrow from other neighbour
     *      2c: If both don't meet req, merge with left neighbour (?)
     *
     *      2a & 2b should prioritise left neighbour if both are from same parent
     */

    if (this->root == nullptr){
        return;
    }

    std::vector<Record*> *r = this->searchRecord(key);
    if (r == nullptr){
        // Case 0: Key does not exist in the tree
        std::cout << "Key " << key << " does not exist in the B+ Tree!" << std::endl;
        return;
    }

    Node *keyNode = this->searchNode(key);
    this->displayKeys(keyNode);
    return;

}