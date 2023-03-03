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
        Node *getNxtLeaf();
        std::vector<int> getKeys();
        std::vector<Record*> getRecords(int idx);
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
        short numOfNodesAcc;

    public:
        Tree(int blockSize);
        ~Tree();
        
        Node* getRoot();
        void setRoot(Node *node);
        int getMaxKeys();
        int getTotalNumOfNodes();
        int getDepth();
        short getBlockSize();
        short getNumOfNodesAcc();
        void setNumOfNodesAcc(int num);
        void displayKeys(Node *node);
        void printTree(Node *node);
        Node* searchNode(int key);
        std::vector<Record*>* searchRecord(int key);
        void insert(int key, Record *recordPtr);
        Node *splitLeafNode(Node *curNode);
        Node *splitInternalNode(Node *curNode, int *key);
        void deleteKey(int key);
        void removeInternal(int key, Node *currentNode, Node *child);
        void updateParentKeys(Node *curNode, Node *parentNode, int parentIdx, std::vector<Node *> &parents, std::vector<int> &prevIdxs);
};

#endif