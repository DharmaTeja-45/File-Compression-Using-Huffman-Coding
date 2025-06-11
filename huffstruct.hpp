#ifndef HUFF_SCT
#define HUFF_SCT
#include <iostream>
using namespace std;

class Node {
    public:

    int freq;
    char character;
    Node *left, *right;
    bool isLeaf;

};

class CompareNode {
    public:
    bool operator()(Node* a, Node* b) {
        return a->freq > b->freq;  // For min-heap
    }
};
#endif