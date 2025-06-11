#ifndef ENC
#define ENC

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <bitset>
#include <vector>
#include <queue>
#include <string>
#include "HuffStruct.hpp"
#include "BuildTree.hpp"

using namespace std;

// Global variables
fstream file;
string txt;
unordered_map<char, string> table;

vector<string> v;            // Temporary path holder during Huffman traversal
string path = "";            // Path built during tree traversal
int needed = 0;              // Padding bits

unsigned char ch = 0;
int bitC = 0;

// Write a single bit to output
void writeBit(int bit) {
    ch = (ch << 1) | bit;
    if (++bitC == 8) {
        file << ch;
        ch = 0;
        bitC = 0;
    }
}

// Search Huffman path for a character
void searchPath(char c, Node *root) {
    if (!root)
        return;

    if (root->isLeaf && c == root->character) {
        v.push_back(path);
        table[root->character] = path;
        return;
    }

    path.push_back('0');
    searchPath(c, root->left);
    path.pop_back();

    path.push_back('1');
    searchPath(c, root->right);
    path.pop_back();
}

// Count total bits to be written
int calculatePadding(unordered_map<char, pair<int, string>> &mp,vector<string> store) {
    int bits = 0;
    for (const string &line : store) {
        for (char c : line) {
            bits += mp[c].second.length();
        }
    }
    return bits;
}

Node *Encode(
    unordered_map<char, pair<int, string>> &mp,
    priority_queue<Node *, vector<Node *>,  CompareNode> &q,
    Node *root,vector<string> store,vector<int> mm
) {
    // Read input text and fill `store`

    // Build Huffman codes
    file.open("compressed.txt", ios::out | ios::trunc);
    for (auto &t : mp) {
        searchPath(t.first, root);
        t.second.second = v[0]; // Save generated code
        v.clear();
    }

    // Compute padding needed to make bits multiple of 8
    int totalBits = calculatePadding(mp,store);
    needed = (8 - (totalBits % 8)) % 8;

    // Encode the input using Huffman codes
    for (size_t t = 0; t < store.size(); t++) {
        if (store[t].empty()) {
            file << endl;
            continue;
        }
        for (char temp : store[t]) {
            ++mm[t];
            string code = mp[temp].second;
            for (char bit : code) {
                writeBit(bit == '1' ? 1 : 0);
            }
        }
    }

    // Pad remaining bits
    if (bitC > 0) {
        ch <<= (8 - bitC);
        file << ch;
    }
    file.close();

    // Write code table
    file.open("table.txt", ios::out | ios::trunc);
    for (const auto &t : table) {
        file << t.first << " " << t.second << endl;
    }
    file.close();

    // Write padding to file
    file.open("meta.txt", ios::out | ios::trunc);
    file << needed << endl;
    file.close();

    // Write line character counts
    file.open("line_lengths.txt", ios::out | ios::trunc);
    for (int val : mm) {
        file << val << " ";
    }
    file.close();

    return root;
}

#endif
