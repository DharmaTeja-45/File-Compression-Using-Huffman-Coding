#ifndef DEC
#define DEC
#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include "HuffStruct.hpp"
#include <bitset>
using namespace std;

void Decode(Node *root) {
    Node *Node = root;
    string str;
    fstream file;

    // Read compressed binary data
    file.open("compressed.txt", ios::in | ios::binary);
    vector<string> decoded;
    while (getline(file, str)) {
        for (auto t : str) {
            int x = (int)t;
            if (x < 0) x += 256;
            decoded.push_back(bitset<8>(x).to_string());
        }
    }
    file.close();

    // Read the Huffman code table
    map<string, char> table;
    file.open("table.txt", ios::in);
    while (getline(file, str)) {
        table[str.substr(2)] = str[0];
    }
    file.close();

    // --- New: Read the needed bits from meta.txt (or hardcode if unavailable) ---
    int needed = 0;
    file.open("meta.txt", ios::in);
    if (file.is_open()) {
        file >> needed;
        file.close();
    } else {
        // fallback if file not found
        needed = 0; // assume no padding for now
    }

    // --- New: Read mm vector (line-wise character counts) ---
    vector<int> mm;
    file.open("line_lengths.txt", ios::in);
    while (file >> str) {
        mm.push_back(stoi(str));
    }
    file.close();

    // Truncate the last byte if it has padding bits
    if (!decoded.empty()) {
        int sz = decoded.size() - 1;
        decoded[sz] = decoded[sz].substr(0, 8 - needed);
    }

    file.open("decoded.txt", ios::out | ios::trunc);
    int currentChars = 0, line = 0;
    string currentPath = "";

    for (const string& g : decoded) {
        for (char tempCh : g) {
            currentPath += tempCh;

            // Maintain original line lengths
            if (line < mm.size() && currentChars == mm[line]) {
                file << '\n';
                currentChars = 0;
                ++line;
            }

            if (tempCh == '0')
                root = root->left;
            else
                root = root->right;

            if (root->isLeaf) {
                file << table[currentPath];
                ++currentChars;
                currentPath = "";
                root = Node;
            }
        }
    }

    file.close();
    return;
}
#endif
