#include "tree.h"

Node::Node(bool isLeaf){
    this->isLeaf = isLeaf;
}

Tree::Tree(int blockSize){
    this->root = nullptr;
    this->blockSize = blockSize;
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

int Tree::getBlockSize(){
    return this->blockSize;
}

void Tree::displayKeys(Node *node){
    std::cout << "{ ";
    for (int key : node->keys){
        std::cout << key << " ";
    }
    std::cout << "}\n";
}

void Tree::printTree(Node *tmp){
    std::vector<Node *> n;
    n.push_back(tmp);
    n.push_back(nullptr);
    while (!n.empty()){
        tmp = n.front();
        n.erase(n.begin());
        if (tmp != nullptr){
            if (tmp->isLeaf) break;
            for (int i = 0; i < tmp->pointers.size(); i++)
            {
                n.push_back(tmp->pointers.at(i));
            }
            n.push_back(nullptr);
            displayKeys(tmp);
        }
        else{
            std::cout << " next level\n";
        }
    }

    while (!tmp->isLeaf){
        tmp = tmp->pointers.at(0);
    }

    do {
        displayKeys(tmp);
        std::cout << " ";
        tmp = tmp->nxtLeaf;
    } while (tmp != nullptr);
    std::cout << "\n";
}