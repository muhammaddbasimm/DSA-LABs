// Lab 11 - Task 1: Huffman Coding using a Binary Heap (Min-Heap)
// CS250 Data Structures and Algorithms
// Name: Muhammad Basim | CMS: 547333 | Class: BSDS 3A

#include <iostream>
#include <string>
using namespace std;

// A node of the Huffman tree
struct Node {
    char ch;        // character (only meaningful for leaf nodes)
    int freq;       // frequency of the character / sum of children
    int order;      // insertion order, used to break ties between equal frequencies
    Node* left;
    Node* right;

    Node(char c, int f, int o) {
        ch = c;
        freq = f;
        order = o;
        left = nullptr;
        right = nullptr;
    }
};

// Binary Min-Heap of Node pointers, written from scratch (array based)
class MinHeap {
private:
    Node** arr;
    int capacity;
    int size;

    int parent(int i) { return (i - 1) / 2; }
    int leftChild(int i) { return 2 * i + 1; }
    int rightChild(int i) { return 2 * i + 2; }

    // a comes before b if it has smaller frequency,
    // and for equal frequency the one inserted first wins
    bool smaller(Node* a, Node* b) {
        if (a->freq != b->freq)
            return a->freq < b->freq;
        return a->order < b->order;
    }

    void swapNodes(int i, int j) {
        Node* temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }

    // move the element at index i up until heap property holds
    void heapifyUp(int i) {
        while (i > 0 && smaller(arr[i], arr[parent(i)])) {
            swapNodes(i, parent(i));
            i = parent(i);
        }
    }

    // move the element at index i down until heap property holds
    void heapifyDown(int i) {
        int smallest = i;
        int l = leftChild(i);
        int r = rightChild(i);

        if (l < size && smaller(arr[l], arr[smallest]))
            smallest = l;
        if (r < size && smaller(arr[r], arr[smallest]))
            smallest = r;

        if (smallest != i) {
            swapNodes(i, smallest);
            heapifyDown(smallest);
        }
    }

public:
    MinHeap(int cap) {
        capacity = cap;
        size = 0;
        arr = new Node*[capacity];
    }

    ~MinHeap() {
        delete[] arr;
    }

    int getSize() { return size; }

    void insert(Node* node) {
        if (size == capacity) {
            cout << "Heap is full" << endl;
            return;
        }
        arr[size] = node;
        heapifyUp(size);
        size++;
    }

    Node* extractMin() {
        if (size == 0)
            return nullptr;
        Node* root = arr[0];
        arr[0] = arr[size - 1];
        size--;
        heapifyDown(0);
        return root;
    }
};

// Build the Huffman tree from characters and their frequencies
Node* buildHuffmanTree(string S, int f[], int n) {
    MinHeap heap(n);
    int counter = 0;

    // step 1: put every character in the heap as a leaf node
    for (int i = 0; i < n; i++) {
        heap.insert(new Node(S[i], f[i], counter++));
    }

    // step 2: keep joining the two smallest nodes until one node is left
    while (heap.getSize() > 1) {
        Node* left = heap.extractMin();
        Node* right = heap.extractMin();

        // internal node: character '$' just marks it as not a leaf
        Node* merged = new Node('$', left->freq + right->freq, counter++);
        merged->left = left;
        merged->right = right;

        heap.insert(merged);
    }

    // the last node is the root of the Huffman tree
    return heap.extractMin();
}

// Preorder traversal: root, left, right
// going left adds '0', going right adds '1'
// the code is printed when we reach a leaf
void printCodes(Node* root, string code) {
    if (root == nullptr)
        return;

    if (root->left == nullptr && root->right == nullptr) {
        // special case: only one character in the input
        if (code == "")
            code = "0";
        cout << root->ch << " : " << code << endl;
        return;
    }

    printCodes(root->left, code + "0");
    printCodes(root->right, code + "1");
}

// free all nodes of the tree
void deleteTree(Node* root) {
    if (root == nullptr)
        return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

int main() {
    int n;
    string S;

    cout << "Enter number of characters (N): ";
    cin >> n;

    cout << "Enter the string of " << n << " distinct characters: ";
    cin >> S;

    if ((int)S.length() != n) {
        cout << "String length does not match N" << endl;
        return 0;
    }

    int* f = new int[n];
    cout << "Enter the " << n << " frequencies: ";
    for (int i = 0; i < n; i++) {
        cin >> f[i];
    }

    Node* root = buildHuffmanTree(S, f, n);

    cout << "\nHuffman codes in preorder traversal:" << endl;
    printCodes(root, "");

    deleteTree(root);
    delete[] f;
    return 0;
}
