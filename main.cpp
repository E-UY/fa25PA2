#include <iostream>
#include <fstream>
#include <stack>
#include <string>
#include "heap.h"

// -------- define static member once --------
int* MinHeap::weightRef = nullptr;

// -------- global node arrays (array-only solution) --------
static const int MAXNODES = 1024; // enough for <= 26 leaves + internal nodes (or more)
char charArr[MAXNODES];           // leaf: 'a'..'z'; internal: '#'
int  weightArr[MAXNODES];         // frequency/weight
int  leftArr[MAXNODES];           // index to left child or -1
int  rightArr[MAXNODES];          // index to right child or -1

// -------- helpers --------
static inline char toLowerAZ(char c) {
    if (c >= 'A' && c <= 'Z') return char(c - 'A' + 'a');
    return c;
}
static inline bool isLowerLetter(char c) { return (c >= 'a' && c <= 'z'); }

// Build Huffman tree from a heap of leaf indices. Returns root index.
int buildEncodingTree(MinHeap &heap, int nextFree) {
    if (heap.size == 1) return heap.pop(); // single-character edge case
    while (heap.size > 1) {
        int a = heap.pop(); // least
        int b = heap.pop(); // next-least
        int parent = nextFree++;
        charArr[parent]   = '#';
        leftArr[parent]   = a;
        rightArr[parent]  = b;
        weightArr[parent] = weightArr[a] + weightArr[b];
        heap.push(parent);
    }
    return heap.pop();
}

// Iteratively assign codes using a stack (no recursion).
void generateCodes(int root, std::string codes[26]) {
    struct Frame { int node; std::string code; };
    std::stack<Frame> st;
    st.push({root, ""});

    while (!st.empty()) {
        Frame f = st.top(); st.pop();
        int node = f.node;
        bool isLeaf = (leftArr[node] == -1 && rightArr[node] == -1);

        if (isLeaf) {
            char ch = charArr[node];
            if (isLowerLetter(ch)) {
                codes[ch - 'a'] = (f.code.empty() ? "0" : f.code); // singleton file -> "0"
            }
        } else {
            // push right first so left is processed first (0 then 1)
            if (rightArr[node] != -1) st.push({ rightArr[node], f.code + "1" });
            if (leftArr[node]  != -1) st.push({ leftArr[node],  f.code + "0" });
        }
    }
}

void encodeMessage(const std::string &filename, std::string codes[26]) {
    // 1) Print the code table
    std::cout << "Character : Code\n";
    for (int i = 0; i < 26; ++i) {
        if (!codes[i].empty()) {
            char ch = char('a' + i);
            std::cout << ch << " : " << codes[i] << "\n";
        }
    }
    std::cout << "\nEncoded message:\n";

    // 2) Re-open and emit encoded bitstring
    std::ifstream fin(filename.c_str());
    if (!fin) {
        std::cout << "(could not open file)\n";
        return;
    }
    std::string out;
    char c;
    while (fin.get(c)) {
        c = toLowerAZ(c);
        if (isLowerLetter(c)) out += codes[c - 'a'];
    }
    std::cout << out << "\n";
}

int main(int argc, char** argv) {
    const std::string filename = (argc > 1 ? argv[1] : "input.txt");

    // 1) frequency table for letters a..z (case-insensitive)
    int freq[26] = {0};
    std::ifstream fin(filename.c_str());
    if (!fin) {
        std::cerr << "Failed to open " << filename << "\n";
        return 1;
    }
    char c;
    while (fin.get(c)) {
        c = toLowerAZ(c);
        if (isLowerLetter(c)) freq[c - 'a']++;
    }
    fin.close();

    // 2) init node arrays
    for (int i = 0; i < MAXNODES; ++i) {
        charArr[i] = '#';
        weightArr[i] = 0;
        leftArr[i] = rightArr[i] = -1;
    }

    // 3) create leaf nodes and heap them by frequency
    MinHeap heap;
    MinHeap::weightRef = weightArr; // IMPORTANT: set comparison array before heap ops

    int nextFree = 0;
    int distinct = 0;
    for (int i = 0; i < 26; ++i) {
        if (freq[i] > 0) {
            int idx = nextFree++;
            charArr[idx] = char('a' + i);
            weightArr[idx] = freq[i];
            leftArr[idx] = rightArr[idx] = -1;
            heap.push(idx);
            ++distinct;
        }
    }

    // If no letters present, still satisfy output contract with empty sections.
    if (distinct == 0) {
        std::cout << "Character : Code\n\nEncoded message:\n\n";
        return 0;
    }

    // 4) combine nodes into a Huffman tree
    int root = buildEncodingTree(heap, nextFree);

    // 5) iterative traversal using a stack to assign codes
    std::string codes[26];
    for (int i = 0; i < 26; ++i) codes[i].clear();
    generateCodes(root, codes);

    // 6) print table and encoded cipher
    encodeMessage(filename, codes);

    return 0;
}
