
#ifndef BLD
#define BLD
#include "HuffStruct.hpp"
using namespace std;

Node *BuildTree(unordered_map<char, pair<int, string> > mp,
        priority_queue<Node *, vector<Node *>,  CompareNode> q) {

    for (auto t=mp.begin();t!=mp.end();t++) {
        Node *hp = new Node();
        hp->freq = t->second.first;
        hp->character = t->first;
        hp->isLeaf = true;
        q.push(hp);
    }

    while (q.size() > 1) {
        Node *hp = new Node();

        Node *tp1 = new Node();
        tp1 = q.top();
        q.pop();

        Node *tp2 = new Node();
        tp2 = q.top();
        q.pop();

        hp->freq = tp1->freq + tp2->freq;
        hp->left = tp1;
        hp->right = tp2;
        hp->isLeaf = false;

        q.push(hp);
    }
    Node *root = q.top();
    q.pop();

    return root;
}
#endif
