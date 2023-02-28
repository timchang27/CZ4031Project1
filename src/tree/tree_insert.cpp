#include "tree.h"

void Tree::insert(int key, Record *recordPtr){
    /**
     * Case 0: Duplicate insertion
     * Case 1: Tree not instantiated
     * Case 2: Inserting key to leaf node
     *      2a: Leaf node keys < max keys
     *      2b: Leaf node keys == max ksys
     * 
     */

    // Case 0: Duplicate insertion
    std::vector<Record*> *records = this->searchRecord(key);
    if (records != nullptr){
        records->push_back(recordPtr);
        return;
    }

    if (this->root == nullptr){
        // Case 1: B+ tree not instantiated
        this->root = new Node(true);
        this->totalNumOfNodes++;
        this->depth++;
        this->root->nxtLeaf = nullptr;
        this->root->keys.push_back(key);
        this->root->records.push_back(std::vector<Record *>(1,recordPtr));
        return;
    }
    
    Node *curNode = this->root;
    std::vector<Node*> parentNodes(1,nullptr);
    int idx = 0;

    // Find the leaf node where the key should be inserted
    while(!curNode->isLeaf){
        idx = std::upper_bound(curNode->keys.begin(), curNode->keys.end(), key) - curNode->keys.begin();
        parentNodes.push_back(curNode);
        curNode = curNode->pointers.at(idx);
    }

    // Insert the key and record into the leaf node at the sorted index
    // Case 2a: Child node keys < max keys. Handled implicitly.
    idx = std::upper_bound(curNode->keys.begin(), curNode->keys.end(), key) - curNode->keys.begin();
    curNode->keys.insert(curNode->keys.begin() + idx, key);
    curNode->records.insert(curNode->records.begin() + idx, std::vector<Record *>(1,recordPtr));
    
    // Case 2b
    if (curNode->keys.size() > this->maxKeys){
        Node *newNode = this->splitLeafNode(curNode);
        Node *parentNode = parentNodes.back();
        parentNodes.pop_back();
        key = newNode->keys.front();

        while (parentNode != nullptr && parentNode->keys.size() == this->maxKeys){
            // Iteratively check if parent is not NULL and has max children
            idx = std::upper_bound(parentNode->keys.begin(), parentNode->keys.end(), key) - parentNode->keys.begin();
            parentNode->keys.insert(parentNode->keys.begin() + idx, key);
            parentNode->pointers.insert(parentNode->pointers.begin() + idx + 1, newNode);

            newNode = this->splitInternalNode(parentNode, &key);
            curNode = parentNode;

            parentNode = parentNodes.back();
            parentNodes.pop_back();
        }

        if (parentNode == nullptr){
            // Root has been reached
            parentNode = new Node(false);
            this->totalNumOfNodes++;
            parentNode->keys.push_back(key);
            parentNode->pointers.push_back(curNode);
            parentNode->pointers.push_back(newNode);
            this->root = parentNode;
            this->depth++;
            return;
        }
        else{
            // Parent node has less than max children, no need split
            idx = std::upper_bound(parentNode->keys.begin(), parentNode->keys.end(), key) - parentNode->keys.begin();
            parentNode->keys.insert(parentNode->keys.begin() + idx, key);
            parentNode->pointers.insert(parentNode->pointers.begin() + idx + 1, newNode);
        }               
    }
    
}

Node *Tree::splitLeafNode(Node *curNode){
    /**
     * Redistribute keys and record pointers among current node and new node
     * Current node will have ceiling(N+1/2) keys
     * New node (split node) will have floor(N+1/2) keys
     */

    Node *splitNode = new Node(true);
    this->totalNumOfNodes++;

    for (int i = 0; i < (this->maxKeys+1)/2; i++){        
        splitNode->keys.insert(splitNode->keys.begin(), curNode->keys.back());
        curNode->keys.pop_back();
        splitNode->records.insert(splitNode->records.begin(), curNode->records.back());
        curNode->records.pop_back();
    }
    splitNode->nxtLeaf = curNode->nxtLeaf;
    curNode->nxtLeaf = splitNode;

    return splitNode;
}

Node *Tree::splitInternalNode(Node *curNode, int *key){
    /**
     * Redistribute keys and record pointers among current node and new node
     * Current node will have ceiling(N/2) keys
     * New node (split node) will have floor(N/2) keys
     */

    Node *splitNode = new Node(false);
    this->totalNumOfNodes++;

    for (int i = 0; i < this->maxKeys/2; i++){
        splitNode->keys.insert(splitNode->keys.begin(), curNode->keys.back());
        curNode->keys.pop_back();
        splitNode->pointers.insert(splitNode->pointers.begin(), curNode->pointers.back());
        curNode->pointers.pop_back();
    }

    splitNode->pointers.insert(splitNode->pointers.begin(), curNode->pointers.back());
    curNode->pointers.pop_back();
    *key = curNode->keys.back();
    curNode->keys.pop_back();
    
    return splitNode;
}