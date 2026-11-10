// Lab 9 - Task 2: Word Frequency Indexing using BST
// Muhammad Basim | CMS 547333 | BSDS 3A | CS-250 DSA
//
// Every node in the BST holds one unique word (stored in lowercase)
// and how many times it appeared. The tree is ordered alphabetically,
// so an inorder walk prints the words A to Z. For the by-frequency
// view the nodes are copied into a second BST ordered by
// (frequency high to low, then word A to Z).

#include <iostream>
#include <string>
#include <cctype>
using namespace std;

struct WordNode {
  string word;
  int count;
  WordNode* left;
  WordNode* right;
  WordNode(const string& w) : word(w), count(1), left(nullptr), right(nullptr) {}
};

// Node for the frequency-ordered tree
struct FreqNode {
  string word;
  int count;
  FreqNode* left;
  FreqNode* right;
  FreqNode(const string& w, int c) : word(w), count(c), left(nullptr), right(nullptr) {}
};

class WordBST {
  WordNode* root;
  int uniqueWords;
  int totalWords;

  WordNode* insert(WordNode* node, const string& w) {
    if (node == nullptr) {
      uniqueWords++;
      return new WordNode(w);
    }
    if (w < node->word) node->left = insert(node->left, w);
    else if (w > node->word) node->right = insert(node->right, w);
    else node->count++;  // word already indexed
    return node;
  }

  void inorder(WordNode* node) const {
    if (node == nullptr) return;
    inorder(node->left);
    cout << "  " << node->word;
    for (int i = node->word.size(); i < 10; i++) cout << " ";
    cout << ": " << node->count << endl;
    inorder(node->right);
  }

  // Ordering for the frequency tree: higher count first,
  // ties broken alphabetically so the output is stable.
  static bool before(int c1, const string& w1, int c2, const string& w2) {
    if (c1 != c2) return c1 > c2;
    return w1 < w2;
  }

  FreqNode* insertFreq(FreqNode* node, const string& w, int c) {
    if (node == nullptr) return new FreqNode(w, c);
    if (before(c, w, node->count, node->word)) node->left = insertFreq(node->left, w, c);
    else node->right = insertFreq(node->right, w, c);
    return node;
  }

  // Copy every word node into the frequency tree
  void buildFreq(WordNode* node, FreqNode*& froot) {
    if (node == nullptr) return;
    froot = insertFreq(froot, node->word, node->count);
    buildFreq(node->left, froot);
    buildFreq(node->right, froot);
  }

  void printFreq(FreqNode* node, int& rank) const {
    if (node == nullptr) return;
    printFreq(node->left, rank);
    cout << (rank < 10 ? "   " : "  ") << rank << ". " << node->word;
    rank++;
    for (int i = node->word.size(); i < 10; i++) cout << " ";
    cout << ": " << node->count << endl;
    printFreq(node->right, rank);
  }

  int maxCount(WordNode* node) const {
    if (node == nullptr) return 0;
    int m = node->count;
    int l = maxCount(node->left);
    int r = maxCount(node->right);
    if (l > m) m = l;
    if (r > m) m = r;
    return m;
  }

  void printWithCount(WordNode* node, int c) const {
    if (node == nullptr) return;
    printWithCount(node->left, c);
    if (node->count == c) cout << "  " << node->word << " (" << c << " times)" << endl;
    printWithCount(node->right, c);
  }

  void destroy(WordNode* node) {
    if (node == nullptr) return;
    destroy(node->left);
    destroy(node->right);
    delete node;
  }

  void destroyFreq(FreqNode* node) {
    if (node == nullptr) return;
    destroyFreq(node->left);
    destroyFreq(node->right);
    delete node;
  }

public:
  WordBST() : root(nullptr), uniqueWords(0), totalWords(0) {}
  ~WordBST() { destroy(root); }

  void insertWord(const string& w) {
    if (w.empty()) return;
    totalWords++;
    root = insert(root, w);
  }

  // Split the paragraph on anything that is not a letter,
  // lowercasing as we go so "The" and "the" are the same key.
  void parseParagraph(const string& text) {
    string cur = "";
    for (char ch : text) {
      if (isalpha((unsigned char)ch)) {
        cur += (char)tolower((unsigned char)ch);
      } else {
        insertWord(cur);
        cur = "";
      }
    }
    insertWord(cur);  // last word if text does not end in punctuation
  }

  void displayAlphabetical() const {
    cout << "Words in alphabetical order:" << endl;
    inorder(root);
  }

  void displayByFrequency() {
    FreqNode* froot = nullptr;
    buildFreq(root, froot);
    cout << "Words by frequency (highest to lowest):" << endl;
    int rank = 1;
    printFreq(froot, rank);
    destroyFreq(froot);
  }

  void findMostFrequentWord() const {
    int m = maxCount(root);
    cout << "Most frequent word(s):" << endl;
    printWithCount(root, m);
  }

  void displaySummary() const {
    cout << "Summary:" << endl;
    cout << "  Total unique words: " << uniqueWords << endl;
    cout << "  Total word count:   " << totalWords << endl;
  }
};

int main() {
  string paragraph =
    "The quick brown fox jumps over the lazy dog. The fox is quick and smart.";

  cout << "=== Task 2: Word Frequency Indexing using BST ===" << endl;
  cout << "Paragraph: \"" << paragraph << "\"" << endl << endl;

  WordBST index;
  index.parseParagraph(paragraph);

  index.displayAlphabetical();
  cout << endl;
  index.displayByFrequency();
  cout << endl;
  index.findMostFrequentWord();
  cout << endl;
  index.displaySummary();
  return 0;
}
