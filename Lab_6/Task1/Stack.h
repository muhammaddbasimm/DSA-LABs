#ifndef STACK_H
#define STACK_H

#include <stdexcept>

// A simple stack made using an array (works for any data type)
template <typename T>
class Stack {
private:
    T* data;
    int topIndex;
    int capacity;

    // make the array bigger when it gets full
    void grow() {
        int newCap = capacity * 2;
        T* newData = new T[newCap];
        for (int i = 0; i <= topIndex; i++) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
        capacity = newCap;
    }

public:
    Stack(int cap = 16) {
        data = new T[cap];
        capacity = cap;
        topIndex = -1;
    }

    ~Stack() {
        delete[] data;
    }

    // put a value on top
    void push(T value) {
        if (topIndex + 1 == capacity) {
            grow();
        }
        topIndex++;
        data[topIndex] = value;
    }

    // remove and return the top value
    T pop() {
        if (isEmpty()) {
            throw std::runtime_error("Stack is empty, cannot pop");
        }
        T value = data[topIndex];
        topIndex--;
        return value;
    }

    // look at the top value without removing it
    T peek() {
        if (isEmpty()) {
            throw std::runtime_error("Stack is empty, cannot peek");
        }
        return data[topIndex];
    }

    bool isEmpty() {
        return topIndex == -1;
    }

    int size() {
        return topIndex + 1;
    }
};

#endif
