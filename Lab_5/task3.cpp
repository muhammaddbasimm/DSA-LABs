#include <iostream>
#include <string>
#include <limits>
using namespace std;

struct Coach {
    int number;
    string type;
    int capacity;
    int passengers;
    Coach* next;
    Coach* prev;
};

class Train {
private:
    Coach* head;
    Coach* current;
    int count;

    void unlink(Coach* node) {
        if (node->next == node) {
            head = nullptr;
            current = nullptr;
        } else {
            node->prev->next = node->next;
            node->next->prev = node->prev;
            if (node == head) head = node->next;
            if (node == current) current = node->next;
        }
        delete node;
        count--;
    }

    void print(const Coach* c) const {
        cout << "Coach " << c->number << ", Type: " << c->type
             << ", Capacity: " << c->capacity << ", Passengers: " << c->passengers
             << ", Empty seats: " << c->capacity - c->passengers << "\n";
    }

public:
    Train() : head(nullptr), current(nullptr), count(0) {}

    ~Train() {
        while (head != nullptr) unlink(head);
    }

    Coach* find(int number) const {
        if (head == nullptr) return nullptr;
        Coach* temp = head;
        do {
            if (temp->number == number) return temp;
            temp = temp->next;
        } while (temp != head);
        return nullptr;
    }

    bool valid(int number, int capacity, int passengers) const {
        if (find(number) != nullptr) {
            cout << "A coach with number " << number << " already exists.\n";
            return false;
        }
        if (capacity < 0 || passengers < 0 || passengers > capacity) {
            cout << "Invalid capacity or passenger count.\n";
            return false;
        }
        return true;
    }

    void addCoach(int number, const string& type, int capacity, int passengers) {
        if (!valid(number, capacity, passengers)) return;
        Coach* node = new Coach{number, type, capacity, passengers, nullptr, nullptr};

        if (head == nullptr) {
            node->next = node;
            node->prev = node;
            head = node;
            current = node;
        } else {
            Coach* tail = head->prev;
            node->prev = tail;
            node->next = head;
            tail->next = node;
            head->prev = node;
        }
        count++;
        cout << "Added coach " << number << ".\n";
    }

    void insertCoach(int afterNumber, int number, const string& type, int capacity, int passengers) {
        Coach* pos = find(afterNumber);
        if (pos == nullptr) {
            cout << "Coach " << afterNumber << " not found.\n";
            return;
        }
        if (!valid(number, capacity, passengers)) return;
        Coach* node = new Coach{number, type, capacity, passengers, nullptr, nullptr};
        node->prev = pos;
        node->next = pos->next;
        pos->next->prev = node;
        pos->next = node;
        count++;
        cout << "Inserted coach " << number << " after coach " << afterNumber << ".\n";
    }

    void removeCoach(int number) {
        Coach* target = find(number);
        if (target == nullptr) {
            cout << "Coach " << number << " not found.\n";
            return;
        }
        unlink(target);
        cout << "Removed coach " << number << ".\n";
    }

    void moveForward() {
        if (current == nullptr) { cout << "Train is empty.\n"; return; }
        current = current->next;
        displayCurrent();
    }

    void moveBackward() {
        if (current == nullptr) { cout << "Train is empty.\n"; return; }
        current = current->prev;
        displayCurrent();
    }

    void displayCurrent() const {
        if (current == nullptr) { cout << "Train is empty.\n"; return; }
        cout << "Current: ";
        print(current);
    }

    void displayClockwise() const {
        if (current == nullptr) { cout << "Train is empty.\n"; return; }
        Coach* temp = current;
        do {
            print(temp);
            temp = temp->next;
        } while (temp != current);
    }

    void displayAntiClockwise() const {
        if (current == nullptr) { cout << "Train is empty.\n"; return; }
        Coach* temp = current;
        do {
            print(temp);
            temp = temp->prev;
        } while (temp != current);
    }

    void searchCoach(int number) const {
        Coach* c = find(number);
        if (c == nullptr) cout << "Coach " << number << " not found.\n";
        else { cout << "Found: "; print(c); }
    }

    void maxAvailableCapacity() const {
        if (head == nullptr) { cout << "Train is empty.\n"; return; }
        Coach* best = head;
        Coach* temp = head->next;
        while (temp != head) {
            if (temp->capacity - temp->passengers > best->capacity - best->passengers)
                best = temp;
            temp = temp->next;
        }
        cout << "Most available capacity: ";
        print(best);
    }

    void reverseTrain() {
        if (head == nullptr) { cout << "Train is empty.\n"; return; }
        Coach* temp = head;
        do {
            Coach* nextNode = temp->next;
            temp->next = temp->prev;
            temp->prev = nextNode;
            temp = nextNode;
        } while (temp != head);
        cout << "Train direction reversed.\n";
    }

    int size() const { return count; }
};

void readCoach(int& number, string& type, int& capacity, int& passengers) {
    cout << "Coach number: "; cin >> number;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Coach type: "; getline(cin, type);
    cout << "Passenger capacity: "; cin >> capacity;
    cout << "Current passengers: "; cin >> passengers;
}

int main() {
    Train train;
    int n;

    cout << "Number of coaches: ";
    cin >> n;
    for (int i = 1; i <= n; i++) {
        int number, capacity, passengers; string type;
        cout << "\nCoach " << i << " of " << n << "\n";
        readCoach(number, type, capacity, passengers);
        train.addCoach(number, type, capacity, passengers);
    }

    int choice;
    do {
        cout << "\nTrain Coach Navigation System\n"
             << "1. Add Coach\n2. Insert Coach After\n3. Remove Coach\n"
             << "4. Move Forward\n5. Move Backward\n6. Display Train Clockwise\n"
             << "7. Display Train Anti-clockwise\n8. Search Coach\n"
             << "9. Find Maximum Available Capacity\n10. Display Current Coach\n"
             << "11. Reverse Train Direction\n0. Exit\nChoice: ";
        cin >> choice;

        if (choice == 1) {
            int number, capacity, passengers; string type;
            readCoach(number, type, capacity, passengers);
            train.addCoach(number, type, capacity, passengers);
        }
        else if (choice == 2) {
            int after, number, capacity, passengers; string type;
            cout << "Insert after coach number: "; cin >> after;
            readCoach(number, type, capacity, passengers);
            train.insertCoach(after, number, type, capacity, passengers);
        }
        else if (choice == 3) {
            int number; cout << "Coach number to remove: "; cin >> number;
            train.removeCoach(number);
        }
        else if (choice == 4) train.moveForward();
        else if (choice == 5) train.moveBackward();
        else if (choice == 6) train.displayClockwise();
        else if (choice == 7) train.displayAntiClockwise();
        else if (choice == 8) {
            int number; cout << "Coach number to search: "; cin >> number;
            train.searchCoach(number);
        }
        else if (choice == 9) train.maxAvailableCapacity();
        else if (choice == 10) train.displayCurrent();
        else if (choice == 11) train.reverseTrain();
        else if (choice != 0) cout << "Invalid choice.\n";
    } while (choice != 0 && cin);

    return 0;
}
