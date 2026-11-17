// Task 3: Hierarchical Cloud Budget & Audit Path Finder (Binary Tree & BST)
#include <iostream>
#include <vector>
#include <string>
using namespace std;

struct Node {
  string serviceID;
  int monthlyCost;
  Node* left;
  Node* right;
  Node(string id, int c) : serviceID(id), monthlyCost(c), left(nullptr), right(nullptr) {}
};

class BST {
public:
  Node* root;
  BST() : root(nullptr) {}
  void insert(string id, int cost) { root = insertRec(root, id, cost); }
  void inorder() { inorderRec(root); cout << "\n"; }
  ~BST() { destroy(root); }
private:
  Node* insertRec(Node* n, string id, int cost) {
    if (!n) return new Node(id, cost);
    if (cost < n->monthlyCost) n->left = insertRec(n->left, id, cost);
    else if (cost > n->monthlyCost) n->right = insertRec(n->right, id, cost);
    return n;  // equal costs ignored (strict BST)
  }
  void inorderRec(Node* n) {
    if (!n) return;
    inorderRec(n->left);
    cout << n->serviceID << "(" << n->monthlyCost << ") ";
    inorderRec(n->right);
  }
  void destroy(Node* n) {
    if (!n) return;
    destroy(n->left); destroy(n->right); delete n;
  }
};

void printNodes(vector<Node*>& v) {
  for (size_t i = 0; i < v.size(); i++)
    cout << v[i]->serviceID << "(" << v[i]->monthlyCost << ")" << (i + 1 < v.size() ? " -> " : "");
}

// ---------- Part A: downward paths with sum B ----------
int pathCount = 0;

// Extend the current path downward from node n
void pathsFrom(Node* n, int sum, int B, vector<Node*>& path) {
  if (!n) return;
  sum += n->monthlyCost;
  if (sum > B) return;           // costs are positive, so going deeper only grows the sum
  path.push_back(n);             // push on the way down
  if (sum == B) {
    pathCount++;
    cout << "  Path " << pathCount << ": ";
    printNodes(path);
    cout << "  = " << B << "\n";
  }
  pathsFrom(n->left, sum, B, path);
  pathsFrom(n->right, sum, B, path);
  path.pop_back();               // pop when backtracking
}

// Try every node as a starting point (preorder)
void allStarts(Node* n, int B) {
  if (!n) return;
  vector<Node*> path;
  pathsFrom(n, 0, B, path);
  allStarts(n->left, B);
  allStarts(n->right, B);
}

// ---------- Part B: subsets of BST nodes with sum B ----------
int subsetCount = 0;
long long visits = 0;

// Walk the BST in order, only over nodes with cost > lastCost.
// Returns true when a "too big" node is hit: that node, its right subtree and
// every later in-order node are larger, so the caller must stop too.
bool subsetWalk(Node* n, Node* root, int lastCost, int cur, int B, vector<Node*>& chosen);

void findSubsets(Node* root, int lastCost, int cur, int B, vector<Node*>& chosen) {
  subsetWalk(root, root, lastCost, cur, B, chosen);
}

bool subsetWalk(Node* n, Node* root, int lastCost, int cur, int B, vector<Node*>& chosen) {
  if (!n) return false;
  visits++;
  if (n->monthlyCost <= lastCost)              // whole left side already used up
    return subsetWalk(n->right, root, lastCost, cur, B, chosen);
  if (subsetWalk(n->left, root, lastCost, cur, B, chosen)) return true;
  if (cur + n->monthlyCost > B) return true;   // PRUNE: skip n and its right subtree
  chosen.push_back(n);                         // include n
  int s = cur + n->monthlyCost;
  if (s == B) {
    subsetCount++;
    cout << "  Subset " << subsetCount << ": { ";
    for (size_t i = 0; i < chosen.size(); i++)
      cout << chosen[i]->serviceID << "(" << chosen[i]->monthlyCost << ") ";
    cout << "}\n";
  } else {
    findSubsets(root, n->monthlyCost, s, B, chosen);  // pick next, larger node
  }
  chosen.pop_back();                           // backtrack
  return subsetWalk(n->right, root, lastCost, cur, B, chosen);
}

void runCase(BST& t, int B) {
  cout << "==================================================\n";
  cout << "Target budget B = " << B << "\n";
  cout << "==================================================\n";
  cout << "Part A: downward paths with total cost B\n";
  pathCount = 0;
  allStarts(t.root, B);
  if (pathCount == 0) cout << "  No downward path found.\n";
  cout << "Part B: subsets of services with total cost B\n";
  subsetCount = 0; visits = 0;
  vector<Node*> chosen;
  findSubsets(t.root, 0, 0, B, chosen);
  if (subsetCount == 0) cout << "  No subset found.\n";
  cout << "  (" << subsetCount << " subsets, " << visits << " node visits with pruning)\n\n";
}

int main() {
  BST t;
  string ids[] = {"API-GW", "AUTH", "DB", "CACHE", "LOG", "QUEUE", "SEARCH",
                  "MAIL", "CDN", "METRIC", "BILLING"};
  int costs[] = {50, 30, 70, 20, 40, 60, 80, 10, 25, 35, 45};
  for (int i = 0; i < 11; i++) t.insert(ids[i], costs[i]);

  cout << "Cloud cost BST (in-order): ";
  t.inorder();
  cout << "\n";
  runCase(t, 100);
  runCase(t, 75);
  return 0;
}
