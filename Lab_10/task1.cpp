// Task 1: Freight Convoy Partitioning (Linked List + Queue + Backtracking)
#include <iostream>
using namespace std;

// ---------- Singly Linked List of containers ----------
struct Container {
  int id;
  int weight;
  Container* next;
  Container(int i, int w) : id(i), weight(w), next(nullptr) {}
};

class ContainerList {
public:
  Container* head;
  Container* tail;
  int count;
  ContainerList() : head(nullptr), tail(nullptr), count(0) {}
  void append(int id, int w) {
    Container* c = new Container(id, w);
    if (!head) head = tail = c;
    else { tail->next = c; tail = c; }
    count++;
  }
  int totalWeight() {
    int s = 0;
    for (Container* p = head; p; p = p->next) s += p->weight;
    return s;
  }
  void print() {
    for (Container* p = head; p; p = p->next)
      cout << "C" << p->id << "(" << p->weight << ")" << (p->next ? " -> " : "");
    cout << " -> NULL\n";
  }
  ~ContainerList() {
    while (head) { Container* t = head; head = head->next; delete t; }
  }
};

// ---------- Truck and circular Queue of trucks ----------
const int MAXC = 50;
struct Truck {
  int id;
  int cap;
  int used;
  int ids[MAXC];
  int n;
};

class TruckQueue {
  Truck* arr[MAXC];
  int front, rear, sz;
public:
  TruckQueue() : front(0), rear(-1), sz(0) {}
  bool isEmpty() { return sz == 0; }
  int size() { return sz; }
  void enqueue(Truck* t) {
    rear = (rear + 1) % MAXC;
    arr[rear] = t;
    sz++;
  }
  Truck* dequeue() {
    Truck* t = arr[front];
    front = (front + 1) % MAXC;
    sz--;
    return t;
  }
};

int freeCapacity = 0;   // total remaining capacity of all trucks
long long branches = 0; // nodes explored, to show pruning effect

// Try to place container 'cur' (and everything after it) into the trucks.
bool backtrack(Container* cur, int remainingWeight, TruckQueue& q) {
  branches++;
  if (cur == nullptr) return true;                // every container placed
  if (remainingWeight > freeCapacity) return false; // prune: not enough space left

  bool triedEmpty = false;
  bool found = false;
  int k = q.size();
  // rotate through the queue once: dequeue a truck, put it back at the rear,
  // then try it (so the deeper call still sees every truck in the queue)
  for (int i = 0; i < k; i++) {
    Truck* t = q.dequeue();
    q.enqueue(t);
    if (!found) {
      bool fits = t->used + cur->weight <= t->cap;  // prune: truck too full
      bool dupEmpty = (t->used == 0 && triedEmpty);  // identical empty trucks
      if (fits && !dupEmpty) {
        if (t->used == 0) triedEmpty = true;
        t->used += cur->weight;                       // choose
        t->ids[t->n++] = cur->id;
        freeCapacity -= cur->weight;
        if (backtrack(cur->next, remainingWeight - cur->weight, q)) found = true;
        else {
          t->used -= cur->weight;                     // undo
          t->n--;
          freeCapacity += cur->weight;
        }
      }
    }
  }
  return found;
}

void runCase(const char* title, int ids[], int w[], int nC, int nTrucks, int W) {
  cout << "==================================================\n";
  cout << title << "\n";
  cout << "==================================================\n";
  ContainerList dock;
  for (int i = 0; i < nC; i++) dock.append(ids[i], w[i]);
  cout << "Dock: ";
  dock.print();

  TruckQueue q;
  Truck* trucks = new Truck[nTrucks];
  for (int i = 0; i < nTrucks; i++) {
    trucks[i].id = i + 1;
    trucks[i].cap = W;
    trucks[i].used = 0;
    trucks[i].n = 0;
    q.enqueue(&trucks[i]);
  }
  freeCapacity = nTrucks * W;
  branches = 0;
  cout << "Trucks in queue: " << nTrucks << ", capacity W = " << W << " tons each\n";
  cout << "Total container weight = " << dock.totalWeight()
       << " tons, total truck capacity = " << freeCapacity << " tons\n\n";

  if (backtrack(dock.head, dock.totalWeight(), q)) {
    cout << "All containers loaded successfully:\n";
    for (int i = 0; i < nTrucks; i++) {
      Truck* t = q.dequeue();
      cout << "  Truck " << t->id << " (" << t->used << "/" << t->cap << " t): ";
      if (t->n == 0) cout << "(empty)";
      for (int j = 0; j < t->n; j++) cout << "C" << t->ids[j] << " ";
      cout << "\n";
      q.enqueue(t);
    }
  } else {
    cout << "Insufficient truck capacity.\n";
  }
  cout << "Recursive calls explored: " << branches << "\n\n";
  delete[] trucks;
}

int main() {
  int ids1[] = {101, 102, 103, 104, 105, 106, 107};
  int w1[] = {4, 8, 1, 4, 2, 1, 7};
  runCase("Case 1: 7 containers, 3 trucks, W = 9", ids1, w1, 7, 3, 9);

  int ids2[] = {201, 202, 203, 204, 205};
  int w2[] = {6, 6, 6, 5, 5};
  runCase("Case 2: 5 containers, 3 trucks, W = 10", ids2, w2, 5, 3, 10);

  int ids3[] = {301, 302, 303, 304};
  int w3[] = {8, 8, 8, 8};
  runCase("Case 3: 4 containers, 2 trucks, W = 15", ids3, w3, 4, 2, 15);
  return 0;
}
