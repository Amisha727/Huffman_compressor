#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <queue>
#include <vector>
#include <bitset>
#include <sstream>

struct Node {
    char ch;
    int freq;
    Node* left;
    Node* right;

    Node(char c, int f) : ch(c), freq(f), left(nullptr), right(nullptr) {}
};

struct Compare {
    bool operator()(Node* a, Node* b) {
        return a->freq > b->freq;
    }
};

void buildCodeMap(Node* root, std::string str, std::unordered_map<char, std::string>& huffmanCode) {
    if (!root) return;
    if (!root->left && !root->right) {
        huffmanCode[root->ch] = str;
    }
    buildCodeMap(root->left, str + "0", huffmanCode);
    buildCodeMap(root->right, str + "1", huffmanCode);
}

// Serialize tree into a string (preorder with markers)
void serialize(Node* root, std::ostream& out) {
    if (!root) {
        out << "# ";
        return;
    }

    if (!root->left && !root->right) {
        out << "L" << root->ch << " ";
    } else {
        out << "I ";
    }

    serialize(root->left, out);
    serialize(root->right, out);
}

// Deserialize tree from string
Node* deserialize(std::istream& in) {
    std::string token;
    in >> token;
    if (token == "#") return nullptr;
    if (token[0] == 'L') return new Node(token[1], 0);
    Node* node = new Node('\0', 0);
    node->left = deserialize(in);
    node->right = deserialize(in);
    return node;
}

#endif
