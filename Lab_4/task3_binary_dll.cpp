#include <iostream>
#include <string>
#include <limits>
using namespace std;

struct BitNode {
  int bit;
  BitNode* next;
  BitNode* prev;
  BitNode(int b) : bit(b), next(nullptr), prev(nullptr) {}
};

class BinaryDLL {
  BitNode* head;
  BitNode* tail;
  int size;

  void pushFront(int b) {
    BitNode* n = new BitNode(b);
    if (head == nullptr) {
      head = tail = n;
    } else {
      n->next = head;
      head->prev = n;
      head = n;
    }
    size++;
  }

  void pushBack(int b) {
    BitNode* n = new BitNode(b);
    if (tail == nullptr) {
      head = tail = n;
    } else {
      n->prev = tail;
      tail->next = n;
      tail = n;
    }
    size++;
  }

  void popFront() {
    if (head == nullptr) return;
    BitNode* t = head;
    head = head->next;
    if (head != nullptr) head->prev = nullptr;
    else tail = nullptr;
    delete t;
    size--;
  }

  void clear() {
    while (head != nullptr) popFront();
  }

  static int roundUp8(int n) {
    return ((n + 7) / 8) * 8;
  }

public:
  BinaryDLL() : head(nullptr), tail(nullptr), size(0) {}

  ~BinaryDLL() {
    clear();
  }

  BinaryDLL(const BinaryDLL& other) : head(nullptr), tail(nullptr), size(0) {
    for (BitNode* p = other.head; p != nullptr; p = p->next) pushBack(p->bit);
  }

  BinaryDLL& operator=(const BinaryDLL& other) {
    if (this != &other) {
      clear();
      for (BitNode* p = other.head; p != nullptr; p = p->next) pushBack(p->bit);
    }
    return *this;
  }

  bool empty() const {
    return head == nullptr;
  }

  void padTo(int width) {
    while (size < width) pushFront(0);
  }

  bool store(const string& bits) {
    if (bits.empty()) return false;
    for (char c : bits)
      if (c != '0' && c != '1') return false;
    clear();
    for (char c : bits) pushBack(c - '0');
    padTo(roundUp8(size));
    return true;
  }

  void display(const string& label) const {
    cout << label << ": ";
    int count = 0;
    for (BitNode* p = head; p != nullptr; p = p->next) {
      cout << p->bit;
      count++;
      if (count % 8 == 0 && p->next != nullptr) cout << " ";
    }
    cout << "  (" << size << " bits)" << endl;
  }

  BinaryDLL onesComplement() const {
    BinaryDLL result;
    for (BitNode* p = head; p != nullptr; p = p->next)
      result.pushBack(p->bit == 0 ? 1 : 0);
    return result;
  }

  static BinaryDLL add(const BinaryDLL& a, const BinaryDLL& b, bool keepCarry) {
    BinaryDLL result;
    BitNode* pa = a.tail;
    BitNode* pb = b.tail;
    int carry = 0;
    while (pa != nullptr || pb != nullptr) {
      int x = (pa != nullptr) ? pa->bit : 0;
      int y = (pb != nullptr) ? pb->bit : 0;
      int sum = x + y + carry;
      result.pushFront(sum % 2);
      carry = sum / 2;
      if (pa != nullptr) pa = pa->prev;
      if (pb != nullptr) pb = pb->prev;
    }
    if (carry == 1 && keepCarry) {
      result.pushFront(1);
      result.padTo(roundUp8(result.size));
    }
    return result;
  }

  BinaryDLL twosComplement() const {
    BinaryDLL ones = onesComplement();
    BinaryDLL one;
    one.store("1");
    return add(ones, one, false);
  }

  void shiftLeft() {
    popFront();
    pushBack(0);
  }

  static BinaryDLL multiply(const BinaryDLL& a, const BinaryDLL& b) {
    int width = roundUp8(a.size + b.size);
    BinaryDLL result;
    result.padTo(width);
    BinaryDLL shifted = a;
    shifted.padTo(width);
    for (BitNode* p = b.tail; p != nullptr; p = p->prev) {
      if (p->bit == 1) result = add(result, shifted, false);
      shifted.shiftLeft();
    }
    return result;
  }

  unsigned long long toDecimal() const {
    unsigned long long value = 0;
    for (BitNode* p = head; p != nullptr; p = p->next)
      value = value * 2 + p->bit;
    return value;
  }
};

void readAndStore(BinaryDLL& num, const string& name) {
  string bits;
  cout << "Enter binary number for " << name << ": ";
  cin >> bits;
  if (num.store(bits)) num.display(name);
  else cout << "Invalid binary number." << endl;
}

int main() {
  BinaryDLL A, B;
  int choice;

  do {
    cout << "\n1. Store number A\n2. Store number B\n3. Display A and B\n";
    cout << "4. 1's complement of A\n5. 2's complement of A\n6. Add A + B\n";
    cout << "7. Multiply A x B\n8. Convert A and B to decimal\n0. Exit\n";
    cout << "Enter choice: ";
    if (!(cin >> choice)) {
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      choice = -1;
    }

    if (choice == 1) readAndStore(A, "A");
    else if (choice == 2) readAndStore(B, "B");
    else if (choice == 3) {
      if (A.empty()) cout << "A: not set" << endl;
      else A.display("A");
      if (B.empty()) cout << "B: not set" << endl;
      else B.display("B");
    }
    else if (choice == 4) {
      if (A.empty()) cout << "Store A first." << endl;
      else {
        A.display("A             ");
        A.onesComplement().display("1's complement");
      }
    }
    else if (choice == 5) {
      if (A.empty()) cout << "Store A first." << endl;
      else {
        A.display("A             ");
        A.twosComplement().display("2's complement");
      }
    }
    else if (choice == 6) {
      if (A.empty() || B.empty()) cout << "Store both A and B first." << endl;
      else BinaryDLL::add(A, B, true).display("A + B");
    }
    else if (choice == 7) {
      if (A.empty() || B.empty()) cout << "Store both A and B first." << endl;
      else BinaryDLL::multiply(A, B).display("A x B");
    }
    else if (choice == 8) {
      if (A.empty() && B.empty()) cout << "Nothing stored yet." << endl;
      if (!A.empty()) cout << "A = " << A.toDecimal() << endl;
      if (!B.empty()) cout << "B = " << B.toDecimal() << endl;
    }
    else if (choice != 0) cout << "Invalid choice." << endl;
  } while (choice != 0);

  return 0;
}
