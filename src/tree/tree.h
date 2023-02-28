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
        std::vector< std::vector<Record*> > records;
    
    public:
        Node(bool isLeaf);
        ~Node();
        // void insertKey(int key);
        // void deleteKey(int key);
        // int getNumOfKeys();

    friend class Tree;
};

class Tree {
    private:
        Node* root;
        int maxKeys;
        int totalNumOfNodes;
        int depth;
        short blockSize;


    public:
        Tree(int blockSize);
        ~Tree();
        
        Node* getRoot();
        void setRoot(Node *node);
        int getMaxKeys();
        int getTotalNumOfNodes();
        int getDepth();
        short getBlockSize();
        void displayKeys(Node *node);
        void printTree(Node *node);
        Node* searchNode(int key);
        std::vector<Record*>* searchRecord(int key);
        void insert(int key, Record *recordPtr);
        Node *splitLeafNode(Node *curNode);
        Node *splitInternalNode(Node *curNode, int *key);
        void deleteNode();
};

#endif