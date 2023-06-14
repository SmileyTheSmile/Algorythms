#include <iostream>
#include <vector>
#include <bitset>
#include <cassert>
#include <fstream>
#include <string>
#include <map>
#include <stack>     
#include <queue>
#include <functional>
//#include "Huffman.h"

typedef unsigned char byte;

struct Node {
    byte value;
    size_t frequency;

    Node* left = nullptr;
    Node* right = nullptr;

    Node();
    Node(size_t occurances): frequency(occurances) {}
    Node(byte symbol, size_t occurances):
         value(symbol), frequency(occurances) {}
    ~Node() {
        delete [] left;
        delete [] right;
    }

    bool operator> (const Node& other) const { return frequency > other.frequency; }
};


#define MAX_TREE_HEIGHT 50
typedef std::priority_queue<Node*, std::deque<Node*>, std::greater<Node*>> MinHeap;


class BitWriter {
    public:
        BitWriter(): bitCount(0)  {}
        
        void WriteBit(byte bit)
        {
            if (bitCount % 8 == 0)
                buffer.push_back(0);
            if (bit)
                buffer[bitCount/8] |= 1 << (7 - bitCount % 8);
            bitCount++;
        }
        
        void WriteByte(byte byte) {
            if (bitCount % 8 == 0) {
                buffer.push_back(byte);
            } else {
                int offset = bitCount % 8;
                buffer[bitCount/8] |= byte >> offset;
                buffer.push_back(byte << (8 - offset));
            }
            bitCount += 8;
        }
        
        const std::vector<byte>& GetBuffer() const {
            return buffer;
        }
        
        size_t GetBitCount() const {
            return bitCount;
        }
        
    private:
        std::vector<byte> buffer;
        size_t bitCount;
};


std::map<char, int> getHuffmanTable(std::ifstream& original) {
    std::map<char, int> huffmanTable;
    std::string line;
    while (std::getline(original, line))
        for (char symbol : line)
            huffmanTable.operator[](symbol)++;
    return huffmanTable;
}

MinHeap huffmanTableToMinHeap(const std::map<char, int>& table) {
    MinHeap heap;
    for(auto const imap: table)
        heap.push(new Node(imap.first, imap.second));
    return heap;
}

Node* minHeapToHuffmanTree(MinHeap& heap) {
    while (heap.size() > 1) {
        Node* left = heap.top(); heap.pop();
        Node* right = heap.top(); heap.pop();
        Node* newNode = new Node(left->frequency + right->frequency);

        newNode->left = left;
        newNode->right = right;

        heap.push(newNode);
    }

    return heap.top();
}

void huffmanTreeToMap(Node* root, int arr[], int top, std::map<byte, std::string> &huffmanCodes) {
    if (root->left) {
        arr[top] = 0;
        huffmanTreeToMap(root->left, arr, top + 1, huffmanCodes);
    }

    if (root->right) {
        arr[top] = 1;
        huffmanTreeToMap(root->right, arr, top + 1, huffmanCodes);
    }

    if (!(root->left) && !(root->right)) {
        std::string code;
        for (int i = 0; i < top; i++)
            code += std::to_string(arr[i]);

        huffmanCodes[root->value] = code;
    }
}


void Encode(std::ifstream& original, std::ofstream& compressed) {
    std::vector<std::string> text;

    std::map<char, int> huffmanTable;
    std::string line;
    while (std::getline(original, line)) {
        for (char symbol : line)
            huffmanTable.operator[](symbol)++;
        text.push_back(line);
    }

    MinHeap nodesHeap = huffmanTableToMinHeap(huffmanTable);
    Node* huffmanTree = minHeapToHuffmanTree(nodesHeap);

    int arr[MAX_TREE_HEIGHT], top = 0;
    std::map<byte, std::string> huffmanCodes;
    huffmanTreeToMap(huffmanTree, arr, top, huffmanCodes);

    for (auto const& imap: huffmanCodes)
        std::cerr << "Key: " << imap.first << ", value: " << imap.second << std::endl;

    BitWriter writer;
    for (std::string line : text) {
        for (char symbol : line) {
            int code = std::stoi(huffmanCodes[symbol]);
            writer.WriteBit()
        }
    }
    //compressed << huffmanCodes[symbol];
}

void Decode(std::ifstream& compressed, std::ofstream& original) {

}


int main(int argc, const char* argv[]) {
    setlocale(0, ".UTF8");

    std::string inputFilePath = "/home/dan/Documents/Projects/Algorythms/input.txt";
    std::string outputFilePath = "/home/dan/Documents/Projects/Algorythms/output.txt";

    std::ifstream inputFile(inputFilePath);
    assert(inputFile.is_open());

    std::ofstream outputFile(outputFilePath);
    assert(outputFile.is_open());

    Encode(inputFile, outputFile);

    return 0;
}