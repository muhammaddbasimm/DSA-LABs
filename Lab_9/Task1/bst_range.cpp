// Lab 9 - Task 1: Range Query and Successor/Predecessor Finder using BST
// Muhammad Basim | CMS 547333 | BSDS 3A | CS-250 DSA
//
// Builds a BST from a fixed sequence, prints the inorder traversal,
// finds the inorder predecessor and successor of a key, and runs a
// range query two ways (recursive and iterative) while counting the
// key comparisons each version makes.

#include <iostream>
using namespace std;

struct Node {
  int key;
  Node* left;
  Node* right;
  Node(int k) : key(k), left(nullptr), right(nullptr) {}
};

// Simple array-based stack for the iterative range query
// (written by hand so no STL container is needed).
// Each entry also remembers the two comparison results for its node,
// so the iterative version never compares the same node twice.
class NodeStack {
  Node* data[100];
  int ca[100], cb[100];
  int top;
public:
  NodeStack() : top(-1) {}
  void push(Node* n, int a, int b) {
    top++;
    data[top] = n;
    ca[top] = a;
    cb[top] = b;
  }
  Node* pop(int& a, int& b) {
    a = ca[top];
    b = cb[top];
    return data[top--];
  }
  bool empty() const { return top == -1; }
};

class BST {
  Node* root;

  Node* insert(Node* node, int key) {
    if (node == nullptr) return new Node(key);
    if (key < node->key) node->left = insert(node->left, key);
    else if (key > node->key) node->right = insert(node->right, key);
    return node;  // duplicates are ignored
  }

  void inorder(Node* node) const {
    if (node == nullptr) return;
    inorder(node->left);
    cout << node->key << " ";
    inorder(node->right);
  }

  // Three-way compare that also counts how many comparisons we make.
  // Returns -1 if a < b, 0 if equal, 1 if a > b.
  static int cmp(int a, int b, int& comps) {
    comps++;
    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
  }

  // Recursive range query with pruning.
  // Each visited node costs exactly 2 comparisons: low vs key and
  // key vs high. Their results decide everything:
  //   low < key  -> left subtree may hold keys in range
  //   key < high -> right subtree may hold keys in range
  void rangeRec(Node* node, int low, int high, int& comps,
                int depth, int& maxDepth) const {
    if (node == nullptr) return;
    if (depth > maxDepth) maxDepth = depth;
    int a = cmp(low, node->key, comps);
    int b = cmp(node->key, high, comps);
    if (a < 0) rangeRec(node->left, low, high, comps, depth + 1, maxDepth);
    if (a <= 0 && b <= 0) cout << node->key << " ";
    if (b < 0) rangeRec(node->right, low, high, comps, depth + 1, maxDepth);
  }

  // Baseline with no pruning: visits every node and checks both bounds.
  void rangeNaive(Node* node, int low, int high, int& comps) const {
    if (node == nullptr) return;
    rangeNaive(node->left, low, high, comps);
    comps += 2;  // low <= key and key <= high
    rangeNaive(node->right, low, high, comps);
  }

  void destroy(Node* node) {
    if (node == nullptr) return;
    destroy(node->left);
    destroy(node->right);
    delete node;
  }

public:
  BST() : root(nullptr) {}
  ~BST() { destroy(root); }

  void insert(int key) { root = insert(root, key); }

  void inorder() const {
    inorder(root);
    cout << endl;
  }

  // Inorder predecessor: largest key smaller than 'key'.
  // Walk down from the root; every time we move right, the current
  // node is a candidate. If the key node has a left subtree, the
  // answer is the maximum of that subtree.
  bool predecessor(int key, int& result) const {
    Node* cur = root;
    Node* pred = nullptr;
    while (cur != nullptr && cur->key != key) {
      if (key > cur->key) {
        pred = cur;
        cur = cur->right;
      } else {
        cur = cur->left;
      }
    }
    if (cur == nullptr) return false;  // key not in tree
    if (cur->left != nullptr) {
      Node* t = cur->left;
      while (t->right != nullptr) t = t->right;
      pred = t;
    }
    if (pred == nullptr) return false;
    result = pred->key;
    return true;
  }

  // Inorder successor: smallest key larger than 'key'.
  bool successor(int key, int& result) const {
    Node* cur = root;
    Node* succ = nullptr;
    while (cur != nullptr && cur->key != key) {
      if (key < cur->key) {
        succ = cur;
        cur = cur->left;
      } else {
        cur = cur->right;
      }
    }
    if (cur == nullptr) return false;
    if (cur->right != nullptr) {
      Node* t = cur->right;
      while (t->left != nullptr) t = t->left;
      succ = t;
    }
    if (succ == nullptr) return false;
    result = succ->key;
    return true;
  }

  // Returns the comparison count; maxDepth reports the deepest
  // recursion level reached (the call stack size).
  int rangeRecursive(int low, int high, int& maxDepth) const {
    int comps = 0;
    maxDepth = 0;
    rangeRec(root, low, high, comps, 1, maxDepth);
    cout << endl;
    return comps;
  }

  int rangeNaiveCount(int low, int high) const {
    int comps = 0;
    rangeNaive(root, low, high, comps);
    return comps;
  }

  // Iterative range query: inorder walk with an explicit stack,
  // pruned the same way as the recursive version. Each node's two
  // comparisons are done once, when it is first reached.
  int rangeIterative(int low, int high, int& maxStack) const {
    int comps = 0;
    maxStack = 0;
    NodeStack st;
    int size = 0;
    Node* cur = root;
    while (cur != nullptr || !st.empty()) {
      while (cur != nullptr) {
        int a = cmp(low, cur->key, comps);
        int b = cmp(cur->key, high, comps);
        st.push(cur, a, b);
        size++;
        if (size > maxStack) maxStack = size;
        cur = (a < 0) ? cur->left : nullptr;
      }
      int a, b;
      Node* n = st.pop(a, b);
      size--;
      if (a <= 0 && b <= 0) cout << n->key << " ";
      cur = (b < 0) ? n->right : nullptr;
    }
    cout << endl;
    return comps;
  }
};

int main() {
  int seq[] = {50, 30, 70, 20, 40, 60, 80, 35, 45, 65, 85};
  int n = sizeof(seq) / sizeof(seq[0]);

  BST tree;
  cout << "=== Task 1: BST Range Query and Successor/Predecessor ===" << endl;
  cout << "Input sequence: ";
  for (int i = 0; i < n; i++) {
    cout << seq[i] << " ";
    tree.insert(seq[i]);
  }
  cout << endl << endl;

  cout << "Inorder traversal: ";
  tree.inorder();
  cout << endl;

  int keys[] = {40, 50, 20, 85};
  for (int k : keys) {
    int p, s;
    cout << "Key = " << k << endl;
    if (tree.predecessor(k, p)) cout << "  Inorder predecessor: " << p << endl;
    else cout << "  Inorder predecessor: none" << endl;
    if (tree.successor(k, s)) cout << "  Inorder successor:   " << s << endl;
    else cout << "  Inorder successor:   none" << endl;
  }
  cout << endl;

  int ranges[][2] = {{30, 70}, {36, 64}, {80, 100}};
  for (auto& r : ranges) {
    int low = r[0], high = r[1];
    int depth, stackMax;
    cout << "Range query [" << low << ", " << high << "]" << endl;
    cout << "  Recursive result: ";
    int c1 = tree.rangeRecursive(low, high, depth);
    cout << "  Iterative result: ";
    int c2 = tree.rangeIterative(low, high, stackMax);
    int c3 = tree.rangeNaiveCount(low, high);
    cout << "  Comparisons -> recursive: " << c1
         << " | iterative: " << c2
         << " | full traversal (no pruning): " << c3 << endl;
    cout << "  Max recursion depth: " << depth
         << " | max explicit stack size: " << stackMax << endl << endl;
  }
  return 0;
}
