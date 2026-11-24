#include <iostream>
using namespace std;

// A node of a singly linked list
struct ListNode {
    int val;
    ListNode* next;

    ListNode(int v) {
        val = v;
        next = nullptr;
    }
};

// Binary Min-Heap of ListNode pointers, ordered by node value (array based)
class MinHeap {
private:
    ListNode** arr;
    int capacity;
    int size;

    int parent(int i) { return (i - 1) / 2; }
    int leftChild(int i) { return 2 * i + 1; }
    int rightChild(int i) { return 2 * i + 2; }

    void swapNodes(int i, int j) {
        ListNode* temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }

    void heapifyUp(int i) {
        while (i > 0 && arr[i]->val < arr[parent(i)]->val) {
            swapNodes(i, parent(i));
            i = parent(i);
        }
    }

    void heapifyDown(int i) {
        int smallest = i;
        int l = leftChild(i);
        int r = rightChild(i);

        if (l < size && arr[l]->val < arr[smallest]->val)
            smallest = l;
        if (r < size && arr[r]->val < arr[smallest]->val)
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
        arr = new ListNode*[capacity];
    }

    ~MinHeap() {
        delete[] arr;
    }

    bool isEmpty() { return size == 0; }

    void insert(ListNode* node) {
        if (size == capacity) {
            cout << "Heap is full" << endl;
            return;
        }
        arr[size] = node;
        heapifyUp(size);
        size++;
    }

    ListNode* extractMin() {
        if (size == 0)
            return nullptr;
        ListNode* root = arr[0];
        arr[0] = arr[size - 1];
        size--;
        heapifyDown(0);
        return root;
    }
};

// Merge k sorted lists into one sorted list
// The heap never holds more than k nodes (one from each list),
// so the time is O(N log k) where N is the total number of nodes
ListNode* mergeKLists(ListNode* lists[], int k) {
    MinHeap heap(k);

    // put the head of every non-empty list in the heap
    for (int i = 0; i < k; i++) {
        if (lists[i] != nullptr)
            heap.insert(lists[i]);
    }

    // dummy node makes it easy to build the result list
    ListNode dummy(0);
    ListNode* tail = &dummy;

    while (!heap.isEmpty()) {
        // the smallest node among all list heads
        ListNode* smallest = heap.extractMin();

        // attach it to the result (we reuse the same nodes, no copying)
        tail->next = smallest;
        tail = smallest;

        // the next node from the same list takes its place in the heap
        if (smallest->next != nullptr)
            heap.insert(smallest->next);
    }

    tail->next = nullptr;
    return dummy.next;
}

// print a linked list like 1 -> 2 -> 3
void printList(ListNode* head) {
    if (head == nullptr) {
        cout << "(empty)" << endl;
        return;
    }
    while (head != nullptr) {
        cout << head->val;
        if (head->next != nullptr)
            cout << " -> ";
        head = head->next;
    }
    cout << endl;
}

// free all nodes of a list
void deleteList(ListNode* head) {
    while (head != nullptr) {
        ListNode* temp = head;
        head = head->next;
        delete temp;
    }
}

int main() {
    int k;
    cout << "Enter number of linked lists (k): ";
    cin >> k;

    if (k <= 0) {
        cout << "Merged list: (empty)" << endl;
        return 0;
    }

    ListNode** lists = new ListNode*[k];

    for (int i = 0; i < k; i++) {
        int n;
        cout << "\nEnter number of elements in list " << i + 1 << ": ";
        cin >> n;

        lists[i] = nullptr;
        ListNode* tail = nullptr;

        if (n > 0)
            cout << "Enter " << n << " elements in ascending order: ";

        for (int j = 0; j < n; j++) {
            int x;
            cin >> x;
            ListNode* node = new ListNode(x);
            if (lists[i] == nullptr)
                lists[i] = node;
            else
                tail->next = node;
            tail = node;
        }
    }

    cout << "\nInput lists:" << endl;
    for (int i = 0; i < k; i++) {
        cout << "List " << i + 1 << ": ";
        printList(lists[i]);
    }

    ListNode* merged = mergeKLists(lists, k);

    cout << "\nMerged sorted list:" << endl;
    printList(merged);

    deleteList(merged);
    delete[] lists;
    return 0;
}
