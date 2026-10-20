

#include <iostream>
#include <string>
#include <cctype>
using namespace std;

struct Node {
  char data;
  Node* left;
  Node* right;
  Node(char d) : data(d), left(nullptr), right(nullptr) {}
};

// Linked-list stack, used as the linear ADT (no std::stack)
template <typename T>
class Stack {
  struct Item {
    T value;
    Item* next;
  };
  Item* head = nullptr;

public:
  ~Stack() {
    while (!isEmpty()) pop();
  }
  void push(T v) { head = new Item{v, head}; }
  T pop() {
    Item* old = head;
    T v = old->value;
    head = old->next;
    delete old;
    return v;
  }
  T top() const { return head->value; }
  bool isEmpty() const { return head == nullptr; }
};



// Grammar: tree = label [ '(' [tree] ',' [tree] ')' ]
// Each open bracket keeps a count of commas seen inside it, and must
// close with exactly one comma (left part, comma, right part).
bool isValidTreeString(const string& s, string& reason) {
  if (s.empty()) {
    reason = "string is empty";
    return false;
  }
  Stack<int> commas;
  char prev = '^';  // '^' means start of string, 'n' means a node label

  for (int i = 0; i < (int)s.size(); i++) {
    char c = s[i];
    string at = " at position " + to_string(i);

    if (isalnum(c)) {
      if (prev != '^' && prev != '(' && prev != ',') {
        reason = "unexpected label '" + string(1, c) + "'" + at;
        return false;
      }
      prev = 'n';
    } else if (c == '(') {
      if (prev != 'n') {
        reason = "'(' must come right after a node label" + at;
        return false;
      }
      commas.push(0);
      prev = '(';
    } else if (c == ',') {
      if (commas.isEmpty() || commas.top() != 0) {
        reason = "misplaced ','" + at;
        return false;
      }
      commas.pop();
      commas.push(1);
      prev = ',';
    } else if (c == ')') {
      if (commas.isEmpty()) {
        reason = "unmatched ')'" + at;
        return false;
      }
      if (commas.pop() != 1) {
        reason = "missing ',' between left and right child" + at;
        return false;
      }
      prev = ')';
    } else {
      reason = "invalid character '" + string(1, c) + "'" + at;
      return false;
    }
  }

  if (!commas.isEmpty()) {
    reason = "missing ')'";
    return false;
  }
  reason = "ok";
  return true;
}



// Builds the subtree starting at s[i]; returns nullptr for an empty child slot
Node* parse(const string& s, int& i) {
  if (i >= (int)s.size() || !isalnum(s[i])) return nullptr;

  Node* node = new Node(s[i++]);
  if (i < (int)s.size() && s[i] == '(') {
    i++;  // skip '('
    node->left = parse(s, i);
    i++;  // skip ','
    node->right = parse(s, i);
    i++;  // skip ')'
  }
  return node;
}

Node* buildTree(const string& s) {
  string reason;
  if (!isValidTreeString(s, reason)) return nullptr;
  int i = 0;
  return parse(s, i);
}

// Prints the tree rotated 90 degrees (right subtree on top)
void printSideways(Node* root, int level = 0) {
  if (!root) return;
  printSideways(root->right, level + 1);
  cout << string(4 * level + 2, ' ') << root->data << "\n";
  printSideways(root->left, level + 1);
}



void preorder(Node* root) {
  if (!root) return;
  cout << root->data << " ";
  preorder(root->left);
  preorder(root->right);
}

// The bracket string is itself a preorder form, so rebuild it recursively
string toTreeString(Node* root) {
  if (!root) return "";
  string s(1, root->data);
  if (root->left || root->right)
    s += "(" + toTreeString(root->left) + "," + toTreeString(root->right) + ")";
  return s;
}



void inorderIterative(Node* root) {
  Stack<Node*> st;
  Node* curr = root;

  while (curr || !st.isEmpty()) {
    // go as far left as possible, saving the path
    while (curr) {
      st.push(curr);
      curr = curr->left;
    }
    curr = st.pop();
    cout << curr->data << " ";
    curr = curr->right;
  }
}



void postorderIterative(Node* root) {
  Stack<Node*> st;
  Node* curr = root;
  Node* lastVisited = nullptr;

  while (curr || !st.isEmpty()) {
    if (curr) {
      st.push(curr);
      curr = curr->left;
      continue;
    }
    Node* peek = st.top();
    // visit the right subtree first if it exists and is not done yet
    if (peek->right && lastVisited != peek->right) {
      curr = peek->right;
    } else {
      cout << peek->data << " ";
      lastVisited = st.pop();
    }
  }
}



// Height counted in edges: a leaf is 0, an empty tree is -1
int height(Node* root) {
  if (!root) return -1;
  return 1 + max(height(root->left), height(root->right));
}

// Depth is passed down from the parent (root is at depth 0)
void printDepths(Node* root, int depth = 0) {
  if (!root) return;
  cout << "  Node " << root->data << " -> depth " << depth
       << ", height " << height(root) << "\n";
  printDepths(root->left, depth + 1);
  printDepths(root->right, depth + 1);
}

void destroy(Node* root) {
  if (!root) return;
  destroy(root->left);
  destroy(root->right);
  delete root;
}



struct BSTNode {
  int key;
  BSTNode* left;
  BSTNode* right;
  BSTNode(int k) : key(k), left(nullptr), right(nullptr) {}
};

BSTNode* bstInsert(BSTNode* root, int key) {
  if (!root) return new BSTNode(key);
  if (key < root->key)
    root->left = bstInsert(root->left, key);
  else if (key > root->key)
    root->right = bstInsert(root->right, key);
  // duplicates are ignored
  return root;
}

// First element becomes the root, the rest are inserted in order
BSTNode* buildBST(const int arr[], int n) {
  BSTNode* root = nullptr;
  for (int i = 0; i < n; i++) root = bstInsert(root, arr[i]);
  return root;
}

void inorderBST(BSTNode* root) {
  if (!root) return;
  inorderBST(root->left);
  cout << root->key << " ";
  inorderBST(root->right);
}

void preorderBST(BSTNode* root) {
  if (!root) return;
  cout << root->key << " ";
  preorderBST(root->left);
  preorderBST(root->right);
}

void destroyBST(BSTNode* root) {
  if (!root) return;
  destroyBST(root->left);
  destroyBST(root->right);
  delete root;
}

void heading(const string& title) {
  cout << "\n--- " << title << " ---\n";
}

int main() {
  string input = "a(b(d,e),c(,f(g,)))";
  cout << "Input string: " << input << "\n";

  heading("1. Build Binary Tree from String");
  Node* root = buildTree(input);
  if (!root) {
    cout << "Could not build tree, string is invalid.\n";
    return 1;
  }
  cout << "Tree built. Structure (rotated, right side on top):\n";
  printSideways(root);

  heading("2. Preorder (Recursive)");
  cout << "Preorder sequence : ";
  preorder(root);
  cout << "\nPreorder string   : " << toTreeString(root) << "\n";

  heading("3. Inorder (Iterative, Stack)");
  cout << "Inorder  : ";
  inorderIterative(root);
  cout << "\n";

  heading("4. Postorder (Iterative, Stack)");
  cout << "Postorder: ";
  postorderIterative(root);
  cout << "\n";

  heading("5. Validate Tree Strings (no tree built)");
  string tests[] = {
    "a(b(d,e),c(,f(g,)))",
    "a(b,c)",
    "a(,)",
    "a(b(d,e),c(,f(g,))",
    "a(b,c,d)",
    "a(b)",
    "a(b,c)d",
    "(a,b)",
    "a(b,c))",
    "a(b#,c)"
  };
  for (const string& t : tests) {
    string reason;
    bool ok = isValidTreeString(t, reason);
    cout << "  " << t << string(22 - t.size(), ' ')
         << (ok ? "VALID" : "INVALID") << (ok ? "" : "  (" + reason + ")") << "\n";
  }

  heading("6. Height and Depth");
  cout << "Height of tree = " << height(root) << " (counted in edges)\n";
  printDepths(root);

  heading("7. BST from Sequence");
  int arr[] = {50, 30, 70, 20, 40, 60, 80};
  int n = sizeof(arr) / sizeof(arr[0]);
  cout << "Sequence : ";
  for (int i = 0; i < n; i++) cout << arr[i] << " ";
  BSTNode* bst = buildBST(arr, n);
  cout << "\nRoot     : " << bst->key;
  cout << "\nInorder  : ";
  inorderBST(bst);
  cout << "\nPreorder : ";
  preorderBST(bst);
  cout << "\n";

  destroy(root);
  destroyBST(bst);
  return 0;
}
