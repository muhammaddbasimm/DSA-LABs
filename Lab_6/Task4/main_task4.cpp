#include <iostream>
#include "WaitingRoom.h"
using namespace std;

// registers a patient and prints the result
void registerPatient(WaitingRoom& w) {
    int id = w.RegisterPatient();
    if (id == -1) {
        cout << "Waiting room is full, cannot register" << endl;
    } else {
        cout << "Registered patient with ID " << id << endl;
    }
}

// serves a patient and prints the result
void servePatient(WaitingRoom& w) {
    int id = w.ServePatient();
    if (id == -1) {
        cout << "No patient to serve" << endl;
    } else {
        cout << "Serving patient with ID " << id << endl;
    }
}

// asks if the doctor can go home
void checkHome(WaitingRoom& w) {
    cout << "Can doctor go home? ";
    if (w.CanDoctorGoHome()) {
        cout << "Yes" << endl;
    } else {
        cout << "No" << endl;
    }
}

int main() {
    WaitingRoom w(5);

    cout << "Waiting Room Management System" << endl;
    checkHome(w);

    cout << endl << "Registering patients" << endl;
    registerPatient(w);
    registerPatient(w);
    registerPatient(w);
    registerPatient(w);
    w.ShowAllPatient();

    cout << endl << "Serving patients" << endl;
    servePatient(w);
    servePatient(w);
    w.ShowAllPatient();

    cout << endl << "Filling the waiting room (capacity 5)" << endl;
    registerPatient(w);
    registerPatient(w);
    registerPatient(w);
    registerPatient(w);   // room is full now
    w.ShowAllPatient();
    checkHome(w);

    cout << endl << "Doctor goes to lunch" << endl;
    w.CancelAll();
    cout << "All appointments cancelled" << endl;
    w.ShowAllPatient();
    checkHome(w);
    servePatient(w);

    cout << endl << "New patient after lunch" << endl;
    registerPatient(w);
    w.ShowAllPatient();
    checkHome(w);

    return 0;
}
