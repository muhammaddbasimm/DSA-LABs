#ifndef WAITINGROOM_H
#define WAITINGROOM_H

#include <iostream>
using namespace std;

// waiting room using a circular array queue of patient ids
class WaitingRoom {
private:
    int* data;
    int frontIndex;
    int count;
    int capacity;
    int nextId;   // the id given to the next patient

public:
    WaitingRoom(int cap = 5) {
        data = new int[cap];
        capacity = cap;
        frontIndex = 0;
        count = 0;
        nextId = 1;
    }

    ~WaitingRoom() {
        delete[] data;
    }

    // gives the patient an id and adds them to the queue
    // returns the id, or -1 if the waiting room is full
    int RegisterPatient() {
        if (count == capacity) {
            return -1;
        }
        int rear = (frontIndex + count) % capacity;
        data[rear] = nextId;
        count++;
        nextId++;
        return data[rear];
    }

    // calls the patient at the front
    // returns the id, or -1 if nobody is waiting
    int ServePatient() {
        if (count == 0) {
            return -1;
        }
        int id = data[frontIndex];
        frontIndex = (frontIndex + 1) % capacity;
        count--;
        return id;
    }

    // removes all waiting patients (ids are not reset)
    void CancelAll() {
        frontIndex = 0;
        count = 0;
    }

    // true if no one is waiting
    bool CanDoctorGoHome() {
        return count == 0;
    }

    // shows the ids of all waiting patients in sorted order
    void ShowAllPatient() {
        cout << "Waiting patients: ";
        if (count == 0) {
            cout << "none" << endl;
            return;
        }

        // copy the ids into a temporary array
        int* temp = new int[count];
        for (int i = 0; i < count; i++) {
            temp[i] = data[(frontIndex + i) % capacity];
        }

        // insertion sort
        for (int i = 1; i < count; i++) {
            int key = temp[i];
            int j = i - 1;
            while (j >= 0 && temp[j] > key) {
                temp[j + 1] = temp[j];
                j--;
            }
            temp[j + 1] = key;
        }

        for (int i = 0; i < count; i++) {
            cout << temp[i] << " ";
        }
        cout << endl;
        delete[] temp;
    }
};

#endif
