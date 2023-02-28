#include "tree.h"

Node::Node(bool isLeaf){
    this->isLeaf = isLeaf;
}

Tree::Tree(int blockSize){
    this->root = nullptr;
    this->maxKeys = (blockSize - sizeof(int*)) / (sizeof(int) + sizeof(int*));
}

Node *Tree::getRoot(){
    return this->root;
}

void Tree::setRoot(Node *r){
    this->root = r;
    return;
}

int Tree::getMaxKeys(){
    return this->maxKeys;
}

int Tree::getNumOfNodes(){
    return this->numOfNodes;
}

void Tree::displayKeys(Node *node){
    std::cout << "{ ";
    for (auto key : node->keys){
        std::cout << key << " ";
    }
    std::cout << " }" << std::endl;
}