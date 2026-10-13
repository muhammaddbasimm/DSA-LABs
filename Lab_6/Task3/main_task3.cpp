#include <iostream>
#include "Queue.h"
using namespace std;

// adds a request and shows the queue
void doEnqueue(CircularQueue& q, int id, int arrival, int processing) {
    PrintRequest r;
    r.id = id;
    r.arrival = arrival;
    r.processing = processing;

    cout << "Enqueue request " << id << " (arrival " << arrival
         << ", processing " << processing << ")" << endl;
    q.enqueue(r);
    q.display();
}

// removes a request and shows the queue
void doDequeue(CircularQueue& q) {
    PrintRequest r = q.dequeue();
    cout << "Dequeue request " << r.id << endl;
    q.display();
}

// the print server simulation
void printServerSimulation() {
    CircularQueue q(4);

    doEnqueue(q, 1, 0, 5);
    doEnqueue(q, 2, 1, 3);
    doEnqueue(q, 3, 2, 4);
    doEnqueue(q, 4, 3, 2);
    doEnqueue(q, 5, 4, 6);
    doEnqueue(q, 6, 5, 1);
    doEnqueue(q, 7, 6, 3);
    doEnqueue(q, 8, 7, 2);
    cout << endl;

    int time = 0;        // the time when the printer becomes free
    int totalWait = 0;
    int processed = 0;

    while (!q.isEmpty()) {
        PrintRequest next = q.front();
        cout << "Dequeue request " << next.id << endl;
        PrintRequest r = q.dequeue();

        // the printer may have to wait for the request to arrive
        if (time < r.arrival) {
            time = r.arrival;
        }
        int wait = time - r.arrival;
        cout << "  Request " << r.id << " starts at " << time
             << ", waited " << wait
             << ", finishes at " << time + r.processing << endl;

        time = time + r.processing;
        totalWait = totalWait + wait;
        processed++;
        q.display();
    }

    cout << endl;
    cout << "Requests processed: " << processed << endl;
    cout << "Total waiting time: " << totalWait << endl;
    cout << "Average waiting time: " << (double)totalWait / processed << endl;
}

// shows the queue wrapping around the end of the array
void wrapAroundTest() {
    CircularQueue q(4);

    doEnqueue(q, 1, 0, 1);
    doEnqueue(q, 2, 0, 1);
    doEnqueue(q, 3, 0, 1);
    doDequeue(q);
    doDequeue(q);
    doEnqueue(q, 4, 0, 1);
    doEnqueue(q, 5, 0, 1); 
    doEnqueue(q, 6, 0, 1);

    cout << "isFull: " << (q.isFull() ? "Yes" : "No") << endl;
    cout << "Front request: " << q.front().id << endl;
}

int main() {
    cout << "Print Server Simulation" << endl;
    printServerSimulation();

    cout << endl;
    cout << "Wrap-around Test" << endl;
    wrapAroundTest();

    cout << endl;
    cout << "Empty Queue Test" << endl;
    CircularQueue empty;
    try {
        empty.dequeue();
    } catch (exception& e) {
        cout << "Error caught: " << e.what() << endl;
    }

    return 0;
}
