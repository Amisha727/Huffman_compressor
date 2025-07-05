#include "utils.hpp"

int main() {
    std::ifstream inFile("compressed.huff", std::ios::binary);
    if (!inFile) {
        std::cerr << "Error opening compressed file.\n";
        return 1;
    }

    int padding = inFile.get();
    if (inFile.eof()) {
        std::cerr << "File too short (padding not found).\n";
        return 1;
    }

    size_t treeLen = 0;
    inFile.read(reinterpret_cast<char*>(&treeLen), sizeof(treeLen));
    if (inFile.gcount() != sizeof(treeLen)) {
        std::cerr << "Tree length read failed.\n";
        return 1;
    }

    std::string treeStr(treeLen, ' ');
    inFile.read(&treeStr[0], treeLen);
    if (inFile.gcount() != treeLen) {
        std::cerr << "Could not read entire tree data.\n";
        return 1;
    }

    std::istringstream treeStream(treeStr);
    Node* root = deserialize(treeStream);
    if (!root) {
        std::cerr << "Tree deserialization failed.\n";
        return 1;
    }

    std::string binaryStr = "";
    char byte;
    while (inFile.get(byte)) {
        std::bitset<8> bits(static_cast<unsigned char>(byte));
        binaryStr += bits.to_string();
    }
    inFile.close();

    if (padding > 0 && padding <= 8 && padding <= binaryStr.size()) {
        binaryStr = binaryStr.substr(0, binaryStr.size() - padding);
    } else {
        std::cerr << "Invalid padding.\n";
        return 1;
    }

    std::ofstream outFile("output.txt", std::ios::binary);
    if (!outFile) {
        std::cerr << "Error creating output.txt\n";
        return 1;
    }

    Node* curr = root;
    for (char bit : binaryStr) {
        if (bit == '0') curr = curr->left;
        else curr = curr->right;

        if (!curr) {
            std::cerr << "Error: Traversal hit NULL. Malformed input.\n";
            return 1;
        }

        if (!curr->left && !curr->right) {
            outFile.put(curr->ch);
            curr = root;
        }
    }

    outFile.close();
    std::cout << "File decompressed successfully to output.txt\n";
    return 0;
}
