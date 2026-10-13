#ifndef QUEUE_H
#define QUEUE_H

#include <iostream>
#include <stdexcept>
using namespace std;

// one print request
struct PrintRequest {
    int id;
    int arrival;
    int processing;
};

// circular queue that grows and shrinks by itself
class CircularQueue {
private:
    PrintRequest* data;
    int frontIndex;
    int count;
    int capacity;
    int minCapacity;   // the queue never shrinks below this

public:
    CircularQueue(int cap = 4) {
        data = new PrintRequest[cap];
        capacity = cap;
        minCapacity = cap;
        frontIndex = 0;
        count = 0;
    }

    ~CircularQueue() {
        delete[] data;
    }

    bool isEmpty() {
        return count == 0;
    }

    bool isFull() {
        return count == capacity;
    }

    int size() {
        return count;
    }

    // moves everything to a new array of the given size
    void resize(int newCapacity) {
        if (newCapacity < count) {
            throw runtime_error("New capacity is too small");
        }
        PrintRequest* newData = new PrintRequest[newCapacity];

        // copy in order, starting from the front
        for (int i = 0; i < count; i++) {
            newData[i] = data[(frontIndex + i) % capacity];
        }

        delete[] data;
        data = newData;
        frontIndex = 0;
        cout << "  Resized: " << capacity << " -> " << newCapacity << endl;
        capacity = newCapacity;
    }

    // adds a request at the back
    void enqueue(PrintRequest x) {
        if (isFull()) {
            resize(capacity * 2);
        }
        int rear = (frontIndex + count) % capacity;
        data[rear] = x;
        count++;
    }

    // removes and returns the request at the front
    PrintRequest dequeue() {
        if (isEmpty()) {
            throw runtime_error("Queue is empty, cannot dequeue");
        }
        PrintRequest x = data[frontIndex];
        frontIndex = (frontIndex + 1) % capacity;
        count--;

        // too much empty space, make the array smaller
        if (capacity > minCapacity && count <= capacity / 4) {
            resize(capacity / 2);
        }
        return x;
    }

    // looks at the front request without removing it
    PrintRequest front() {
        if (isEmpty()) {
            throw runtime_error("Queue is empty, no front");
        }
        return data[frontIndex];
    }

    // prints the current state of the queue
    void display() {
        cout << "  Queue (size " << count << ", capacity " << capacity
             << ", front index " << frontIndex << "): ";
        if (isEmpty()) {
            cout << "empty";
        }
        for (int i = 0; i < count; i++) {
            cout << data[(frontIndex + i) % capacity].id << " ";
        }
        cout << endl;
    }
};

#endif
