#include "utils.hpp"

int main() {
    std::ifstream inFile("input.txt", std::ios::binary);
    if (!inFile) {
        std::cerr << "Error opening input file.\n";
        return 1;
    }

    std::unordered_map<char, int> freq;
    char ch;
    std::string text;

    while (inFile.get(ch)) {
        freq[ch]++;
        text += ch;
    }
    inFile.close();

    std::priority_queue<Node*, std::vector<Node*>, Compare> pq;
    for (auto& pair : freq)
        pq.push(new Node(pair.first, pair.second));

    while (pq.size() > 1) {
        Node* left = pq.top(); pq.pop();
        Node* right = pq.top(); pq.pop();
        Node* merged = new Node('\0', left->freq + right->freq);
        merged->left = left;
        merged->right = right;
        pq.push(merged);
    }

    Node* root = pq.top();

    std::unordered_map<char, std::string> codeMap;
    buildCodeMap(root, "", codeMap);

    std::string encodedStr = "";
    for (char c : text) {
        encodedStr += codeMap[c];
    }

    int padding = 8 - (encodedStr.size() % 8);
    if (padding != 8) encodedStr.append(padding, '0');

    std::ofstream outFile("compressed.huff", std::ios::binary);
    outFile.put(padding);

    std::ostringstream treeStream;
    serialize(root, treeStream);
    std::string treeStr = treeStream.str();
    size_t treeLen = treeStr.size();

    outFile.write(reinterpret_cast<char*>(&treeLen), sizeof(treeLen));
    outFile.write(treeStr.c_str(), treeLen);

    for (size_t i = 0; i < encodedStr.size(); i += 8) {
        std::bitset<8> byte(encodedStr.substr(i, 8));
        outFile.put(static_cast<unsigned char>(byte.to_ulong()));
    }

    outFile.close();
    std::cout << "File compressed successfully to compressed.huff\n";
    return 0;
}
