#ifndef TREE_H
#define TREE_H

#include <vector>
#include "../storage/disk.h"

class Node {
    private:
        bool isLeaf;
        Node *nxtLeaf;
        std::vector<Node*> pointers;
        std::vector<int> keys;
        std::vector<std::vector<Record*>> records;
    
    public:
        Node(bool isLeaf);
        Node(){}
        ~Node(){}
        // void insertKey(int key);
        // void deleteKey(int key);
        // int getNumOfKeys();

    friend class Tree;
};

class Tree {
    private:
        Node* root;
        int maxKeys;
        int numOfNodes;

    public:
        Tree(int blockSize);
        ~Tree(){}
        
        Node* getRoot();
        void setRoot(Node *node);
        int getMaxKeys();
        int getNumOfNodes();
        void displayKeys(Node *node);
        int getDepth();
        Node* searchNode(int key);
        std::vector<Record*>* searchRecord(int key);
        void insert(int key, Record *recordPtr);
        Node *splitLeafNode(Node *curNode);
        Node *splitInternalNode(Node *curNode, int *key);
        void deleteNode();
};

#endif