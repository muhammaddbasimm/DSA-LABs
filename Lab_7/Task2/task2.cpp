
// Records are ordered by studentID.

#include <iostream>
#include <iomanip>
#include <string>
#include <limits>
#include <cstdlib>
using namespace std;

struct Student {
  int studentID;
  string name;
  float GPA;
  Student* left;
  Student* right;
};

class StudentBST {
  Student* root = nullptr;

  Student* insert(Student* node, Student* s, bool& added) {
    if (!node) {
      added = true;
      return s;
    }
    if (s->studentID < node->studentID)
      node->left = insert(node->left, s, added);
    else if (s->studentID > node->studentID)
      node->right = insert(node->right, s, added);
    return node;  // equal ID: not added
  }

  Student* search(Student* node, int id) const {
    while (node && node->studentID != id)
      node = (id < node->studentID) ? node->left : node->right;
    return node;
  }

  Student* findMin(Student* node) const {
    while (node->left) node = node->left;
    return node;
  }

  Student* remove(Student* node, int id, bool& removed) {
    if (!node) return nullptr;

    if (id < node->studentID) {
      node->left = remove(node->left, id, removed);
    } else if (id > node->studentID) {
      node->right = remove(node->right, id, removed);
    } else {
      removed = true;
      // zero or one child: replace the node with its child
      if (!node->left || !node->right) {
        Student* child = node->left ? node->left : node->right;
        delete node;
        return child;
      }
      // two children: copy the inorder successor, then delete it from the right
      Student* succ = findMin(node->right);
      node->studentID = succ->studentID;
      node->name = succ->name;
      node->GPA = succ->GPA;
      bool dummy = false;
      node->right = remove(node->right, succ->studentID, dummy);
    }
    return node;
  }

  void inorder(Student* node) const {
    if (!node) return;
    inorder(node->left);
    printRow(node);
    inorder(node->right);
  }

  int count(Student* node) const {
    if (!node) return 0;
    return 1 + count(node->left) + count(node->right);
  }

  // GPA is not the key, so the whole tree has to be visited
  void findExtremes(Student* node, float& high, float& low) const {
    if (!node) return;
    if (node->GPA > high) high = node->GPA;
    if (node->GPA < low) low = node->GPA;
    findExtremes(node->left, high, low);
    findExtremes(node->right, high, low);
  }

  // Prints students with GPA equal to target (handles ties)
  void printWithGPA(Student* node, float target) const {
    if (!node) return;
    printWithGPA(node->left, target);
    if (node->GPA == target) printRow(node);
    printWithGPA(node->right, target);
  }

  void aboveThreshold(Student* node, float t, int& found) const {
    if (!node) return;
    aboveThreshold(node->left, t, found);
    if (node->GPA > t) {
      printRow(node);
      found++;
    }
    aboveThreshold(node->right, t, found);
  }

  void destroy(Student* node) {
    if (!node) return;
    destroy(node->left);
    destroy(node->right);
    delete node;
  }

  static void printHeader() {
    cout << "  " << left << setw(8) << "ID" << setw(20) << "Name" << "GPA\n";
    cout << "  " << string(32, '-') << "\n";
  }

  static void printRow(Student* s) {
    cout << "  " << left << setw(8) << s->studentID << setw(20) << s->name
         << fixed << setprecision(2) << s->GPA << "\n";
  }

public:
  ~StudentBST() { destroy(root); }

  bool Insert(int id, const string& name, float gpa) {
    Student* s = new Student{id, name, gpa, nullptr, nullptr};
    bool added = false;
    root = insert(root, s, added);
    if (!added) delete s;
    return added;
  }

  void Search(int id) const {
    Student* s = search(root, id);
    if (!s) {
      cout << "  No student found with ID " << id << ".\n";
      return;
    }
    cout << "  Student found:\n";
    printHeader();
    printRow(s);
  }

  bool Delete(int id) {
    bool removed = false;
    root = remove(root, id, removed);
    return removed;
  }

  void displayInorder() const {
    if (!root) {
      cout << "  No records.\n";
      return;
    }
    printHeader();
    inorder(root);
  }

  int countStudents() const { return count(root); }

  void showHighestLowest() const {
    if (!root) {
      cout << "  No records.\n";
      return;
    }
    float high = root->GPA, low = root->GPA;
    findExtremes(root, high, low);
    cout << "  Highest GPA:\n";
    printHeader();
    printWithGPA(root, high);
    cout << "\n  Lowest GPA:\n";
    printHeader();
    printWithGPA(root, low);
  }

  void showAboveThreshold(float t) const {
    cout << "  Students with GPA above " << fixed << setprecision(2) << t << ":\n";
    printHeader();
    int found = 0;
    aboveThreshold(root, t, found);
    if (found == 0) cout << "  (none)\n";
    cout << "  Total: " << found << "\n";
  }
};

// Keeps asking until a valid number in range is entered
template <typename T>
T readNumber(const string& prompt, T lo, T hi) {
  T x;
  while (true) {
    cout << prompt;
    if (cin >> x && x >= lo && x <= hi) {
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      return x;
    }
    if (cin.eof()) {
      cout << "\nInput ended. Exiting.\n";
      exit(0);
    }
    cout << "  Invalid input, try again.\n";
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
  }
}

void showMenu() {
  cout << "\n============== Student Record System (BST) ==============\n"
       << "1. Insert   2. Search by ID   3. Delete by ID   4. Display all\n"
       << "5. Count    6. High/Low GPA   7. GPA above threshold   0. Exit\n";
}

int main() {
  StudentBST db;

  // starting records, inserted in mixed order so the tree is not a chain
  db.Insert(547350, "Ali Raza", 3.45f);
  db.Insert(547320, "Ayesha Khan", 3.82f);
  db.Insert(547380, "Hamza Iqbal", 2.71f);
  db.Insert(547310, "Fatima Noor", 3.10f);
  db.Insert(547333, "Muhammad Basim", 3.67f);
  db.Insert(547365, "Usman Tariq", 2.95f);
  db.Insert(547395, "Zainab Ahmed", 3.82f);
  db.Insert(547301, "Bilal Hussain", 2.40f);
  cout << "Loaded " << db.countStudents() << " student records.\n";

  while (true) {
    showMenu();
    int choice = readNumber<int>("Enter choice: ", 0, 7);
    cout << "\n";

    if (choice == 0) {
      cout << "Exiting. Goodbye!\n";
      break;
    }

    if (choice == 1) {
      int id = readNumber<int>("Enter student ID: ", 1, 99999999);
      cout << "Enter name: ";
      string name;
      getline(cin, name);
      float gpa = readNumber<float>("Enter GPA (0 - 4): ", 0.0f, 4.0f);
      if (db.Insert(id, name, gpa))
        cout << "  Student " << id << " inserted.\n";
      else
        cout << "  ID " << id << " already exists, not inserted.\n";
    } else if (choice == 2) {
      db.Search(readNumber<int>("Enter ID to search: ", 1, 99999999));
    } else if (choice == 3) {
      int id = readNumber<int>("Enter ID to delete: ", 1, 99999999);
      if (db.Delete(id))
        cout << "  Student " << id << " deleted.\n";
      else
        cout << "  No student found with ID " << id << ".\n";
    } else if (choice == 4) {
      db.displayInorder();
    } else if (choice == 5) {
      cout << "  Total students: " << db.countStudents() << "\n";
    } else if (choice == 6) {
      db.showHighestLowest();
    } else if (choice == 7) {
      db.showAboveThreshold(readNumber<float>("Enter GPA threshold: ", 0.0f, 4.0f));
    }
  }
  return 0;
}
