#include <iostream>
#include <limits>
#include <string>
using namespace std;

struct Person {
  int id;
  Person* next;
  Person(int i) : id(i), next(nullptr) {}
};

Person* createCircle(int n, Person*& tail) {
  Person* head = new Person(1);
  tail = head;
  for (int i = 2; i <= n; i++) {
    Person* node = new Person(i);
    tail->next = node;
    tail = node;
  }
  tail->next = head;
  return head;
}

void displayCircle(Person* head) {
  Person* p = head;
  do {
    cout << p->id << " ";
    p = p->next;
  } while (p != head);
  cout << "(back to " << head->id << ")" << endl;
}

int josephus(int n, int k) {
  Person* tail = nullptr;
  Person* curr = createCircle(n, tail);
  Person* prev = tail;

  cout << "Circle: ";
  displayCircle(curr);
  cout << "\nElimination order:" << endl;

  int round = 1;
  while (curr->next != curr) {
    for (int step = 1; step < k; step++) {
      prev = curr;
      curr = curr->next;
    }
    cout << "Round " << round << ": person " << curr->id << " eliminated" << endl;
    round++;
    prev->next = curr->next;
    delete curr;
    curr = prev->next;
  }

  int survivor = curr->id;
  delete curr;
  return survivor;
}

int readPositive(string prompt) {
  int x;
  cout << prompt;
  while (!(cin >> x) || x < 1) {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Enter a positive number. " << prompt;
  }
  return x;
}

int main() {
  int n = readPositive("Enter number of people (N): ");
  int k = readPositive("Enter step count (k): ");
  int survivor = josephus(n, k);
  cout << "\nSurvivor: person " << survivor << endl;
  return 0;
}
