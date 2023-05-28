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
#define MAX_TREE_HEIGHT 50

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

typedef std::priority_queue<Node*, std::deque<Node*>, std::greater<Node*>> MinHeap;


class BitWriter {
    public:
        BitWriter(): bitCount(0)  {}
        
        void WriteBit(unsigned char bit)
        {
            if (bitCount % 8 == 0)
                buffer.push_back(0);
            if (bit)
                buffer[bitCount/8] |= 1 << (7 - bitCount % 8);
            bitCount++;
        }
        
        void WriteByte(unsigned char byte) {
            if (bitCount % 8 == 0) {
                buffer.push_back(byte);
            } else {
                int offset = bitCount % 8;
                buffer[bitCount/8] |= byte >> offset;
                buffer.push_back(byte << (8 - offset));
            }
            bitCount += 8;
        }
        
        const std::vector<unsigned char>& GetBuffer() const {
            return buffer;
        }
        
        size_t GetBitCount() const {
            return bitCount;
        }
        
    private:
        std::vector<unsigned char> buffer;
        size_t bitCount;
};

void visualizeBuffer(const std::vector<unsigned char>& buffer) {
    for (auto &b: buffer)
        std::cout << std::bitset<8>(b) << "|";
    std::cout << std::endl;
}


void printHuffmanTable(std::map<char, int> table) {
    for(auto const& imap: table)
        std::cerr << "Key: " << imap.first << ", value: " << imap.second << std::endl;
}

std::map<char, int> getHuffmanTable(std::ifstream& original) {
    std::map<char, int> huffmanTable;
    std::string line;
    while (std::getline(original, line))
        for (char symbol : line)
            huffmanTable.operator[](symbol)++;
    return huffmanTable;
}


void printHuffmanMinHeap(MinHeap heap) {
    while (!heap.empty()) {
        Node* node = heap.top(); heap.pop();
        std::cerr << "Key: " << node->value << ", value: " << node->frequency << std::endl;
    }
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

void traversePostorder(Node* tree) {
    if (tree->left)
        traversePostorder(tree->left);
    std::cout << " " << tree->frequency << " " << std::endl;
    if (tree->right)
        traversePostorder(tree->right);
}

void printArray(int arr[], int n, std::ofstream& compressed) {
    for (int i = 0; i < n; ++i)
        compressed << arr[i];
    compressed << "\n";
}

void printHCodes(Node* root, int arr[], int top, std::ofstream& compressed) {
  if (root->left) {
    arr[top] = 0;
    printHCodes(root->left, arr, top + 1, compressed);
  }

  if (root->right) {
    arr[top] = 1;
    printHCodes(root->right, arr, top + 1, compressed);
  }

  if (!(root->left) && !(root->right)) {
    compressed << root->value << "  | ";
    printArray(arr, top, compressed);
  }
}


void Encode(std::ifstream& original, std::ofstream& compressed) {
    std::map<char, int> huffmanTable = getHuffmanTable(original);
    MinHeap nodesHeap = huffmanTableToMinHeap(huffmanTable);
    Node* huffmanTree = minHeapToHuffmanTree(nodesHeap);

    int arr[MAX_TREE_HEIGHT], top = 0;
    printHCodes(huffmanTree, arr, top, compressed);
}

void Decode(std::ifstream& compressed, std::ofstream& original) {

}


int main(int argc, const char* argv[]) {
    setlocale(0, ".UTF8");

    std::string inputFilePath = "/home/dan/Documents/Projects/Algorythms/input.txt";
    std::string outputFilePath = "/home/dan/Documents/Projects/Algorythms/output.txt";

    std::ifstream inputFile(inputFilePath);
    std::ofstream outputFile(outputFilePath);
    assert(inputFile.is_open());

    Encode(inputFile, outputFile);

    return 0;
}