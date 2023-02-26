#include "tree.h"

void Tree::insert(int key, Record *recordPtr){
    // Case 0: Duplicate insertion
    std::vector<Record*> *records = this->searchRecord(key);
    if (records != nullptr){
        records->push_back(recordPtr);
        return;
    }

    if (this->root == nullptr){
        // Case 1: B+ tree not instantiated
        this->root = new Node(true);
        this->numOfNodes++;
        this->root->nxtLeaf = nullptr;
        this->root->keys.push_back(key);
        this->root->records.push_back(std::vector<Record*>{recordPtr});
        return;
    }
    else{
        Node *curNode = this->root;
        std::vector<Node*> parentNodes{nullptr};
        int idx{0};

        // Find the leaf node where the key should be inserted
        while(!curNode->isLeaf){
            idx = std::upper_bound(curNode->keys.begin(), curNode->keys.end(), key) - curNode->keys.begin();
            parentNodes.push_back(curNode);
            curNode = curNode->pointers.at(idx);
        }

        // Insert the key and record into the leaf node at the sorted index
        idx = std::upper_bound(curNode->keys.begin(), curNode->keys.end(), key) - curNode->keys.begin();
        curNode->keys.insert(curNode->keys.begin() + idx, key);
        curNode->records.insert(curNode->records.begin() + idx, std::vector<Record *>{recordPtr});
        
        if (curNode->keys.size() <= this->maxKeys){
            // Case 2: Child node keys < max keys. Handled implicitly.
            return;
        }
        else if (curNode->keys.size() > this->maxKeys){
            Node *newNode = this->splitLeafNode(curNode);
            Node *parentNode = parentNodes.back();
            parentNodes.pop_back();
            key = newNode->keys.front();

            while (parentNode != nullptr && parentNode->keys.size() == this->maxKeys){
                // Case 3b: Parent has max children, split parent
                idx = std::upper_bound(parentNode->keys.begin(), parentNode->keys.end(), key) - parentNode->keys.begin();
                parentNode->pointers.insert(parentNode->pointers.begin() + idx + 1, newNode);
                parentNode->keys.insert(parentNode->keys.begin() + idx, key);

                newNode = this->splitInternalNode(parentNode, &key);
                parentNode = parentNodes.back();
                parentNodes.pop_back();
            }

            if (parentNode == nullptr){
                // Root has been reached
                parentNode = new Node(false);
                this->numOfNodes++;
                parentNode->keys.push_back(newNode->keys.front());
                parentNode->pointers.push_back(curNode);
                parentNode->pointers.push_back(newNode);
                this->root = parentNode;
                return;
            }
            else{
                // Case 3a: Parent node has less than max children, no need split
                idx = std::upper_bound(parentNode->keys.begin(), parentNode->keys.end(), key) - parentNode->keys.begin();
                parentNode->keys.insert(parentNode->keys.begin() + idx, key);
                parentNode->pointers.insert(parentNode->pointers.begin() + idx, newNode);
            }               
        }
    }
}

Node *Tree::splitLeafNode(Node *curNode){
    /**
     * Redistribute keys and record pointers among current node and new node
     * Current node will have floor(N+1/2) keys
     * New node (split node) will have ceiling(N+1/2) keys
     */

    Node *splitNode = new Node(true);
    this->numOfNodes++;

    for (int i{0}; i < (this->maxKeys+1)/2; i++){        
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
     * Current node will have floor(N/2) keys
     * New node (split node) will have ceiling(N/2) keys
     */

    Node *splitNode = new Node(false);
    this->numOfNodes++;
   
    for (int i{0}; i < this->maxKeys/2 - 1; i++){
        splitNode->keys.insert(splitNode->keys.begin(), curNode->keys.back());
        curNode->keys.pop_back();
        splitNode->pointers.insert(splitNode->pointers.begin(), curNode->pointers.back());
        curNode->pointers.pop_back();
    }

    *key = curNode->keys.back();
    curNode->keys.pop_back();
    
    return splitNode;
}