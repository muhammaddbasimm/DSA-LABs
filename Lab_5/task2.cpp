#include <iostream>
#include <string>
#include <limits>
using namespace std;

struct Photo {
    int id;
    string name;
    string date;
    string location;
    Photo* next;
    Photo* prev;
};

class Album {
private:
    Photo* head;
    Photo* current;
    int count;

    void unlink(Photo* node) {
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

public:
    Album() : head(nullptr), current(nullptr), count(0) {}

    ~Album() {
        while (head != nullptr) unlink(head);
    }

    Photo* find(int id) const {
        if (head == nullptr) return nullptr;
        Photo* temp = head;
        do {
            if (temp->id == id) return temp;
            temp = temp->next;
        } while (temp != head);
        return nullptr;
    }

    void addPhoto(int id, const string& name, const string& date, const string& loc) {
        if (find(id) != nullptr) {
            cout << "A photo with ID " << id << " already exists.\n";
            return;
        }
        Photo* node = new Photo{id, name, date, loc, nullptr, nullptr};

        if (head == nullptr) {
            node->next = node;
            node->prev = node;
            head = node;
            current = node;
        } else {
            Photo* tail = head->prev;
            node->prev = tail;
            node->next = head;
            tail->next = node;
            head->prev = node;
        }
        count++;
        cout << "Added photo " << id << ".\n";
    }

    void insertAfterCurrent(int id, const string& name, const string& date, const string& loc) {
        if (head == nullptr) {
            addPhoto(id, name, date, loc);
            return;
        }
        if (find(id) != nullptr) {
            cout << "A photo with ID " << id << " already exists.\n";
            return;
        }
        Photo* node = new Photo{id, name, date, loc, nullptr, nullptr};
        node->prev = current;
        node->next = current->next;
        current->next->prev = node;
        current->next = node;
        count++;
        cout << "Inserted photo " << id << " after the current photo.\n";
    }

    void removeById(int id) {
        Photo* target = find(id);
        if (target == nullptr) {
            cout << "Photo " << id << " not found.\n";
            return;
        }
        unlink(target);
        cout << "Removed photo " << id << ".\n";
    }

    void removeCurrent() {
        if (current == nullptr) { cout << "Album is empty.\n"; return; }
        int id = current->id;
        unlink(current);
        cout << "Removed photo " << id << ".\n";
    }

    void moveNext() {
        if (current == nullptr) { cout << "Album is empty.\n"; return; }
        current = current->next;
        displayCurrent();
    }

    void movePrevious() {
        if (current == nullptr) { cout << "Album is empty.\n"; return; }
        current = current->prev;
        displayCurrent();
    }

    void displayCurrent() const {
        if (current == nullptr) { cout << "Album is empty.\n"; return; }
        cout << "Current photo - ID: " << current->id << ", Name: " << current->name
             << ", Date: " << current->date << ", Location: " << current->location << "\n";
    }

    void displayForward() const {
        if (current == nullptr) { cout << "Album is empty.\n"; return; }
        Photo* temp = current;
        do {
            cout << "ID: " << temp->id << ", Name: " << temp->name
                 << ", Date: " << temp->date << ", Location: " << temp->location << "\n";
            temp = temp->next;
        } while (temp != current);
    }

    void displayBackward() const {
        if (current == nullptr) { cout << "Album is empty.\n"; return; }
        Photo* temp = current;
        do {
            cout << "ID: " << temp->id << ", Name: " << temp->name
                 << ", Date: " << temp->date << ", Location: " << temp->location << "\n";
            temp = temp->prev;
        } while (temp != current);
    }

    void searchPhoto(int id) const {
        Photo* p = find(id);
        if (p == nullptr) cout << "Photo " << id << " not found.\n";
        else cout << "Found - ID: " << p->id << ", Name: " << p->name
                  << ", Date: " << p->date << ", Location: " << p->location << "\n";
    }

    void countPhotos() const {
        cout << "Total photos: " << count << "\n";
    }
};

void readPhoto(int& id, string& name, string& date, string& loc) {
    cout << "Photo ID: "; cin >> id;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Photo name: "; getline(cin, name);
    cout << "Date taken: "; getline(cin, date);
    cout << "Location: "; getline(cin, loc);
}

int main() {
    Album album;
    int n;

    cout << "Number of photos: ";
    cin >> n;
    for (int i = 1; i <= n; i++) {
        int id; string name, date, loc;
        cout << "\nPhoto " << i << " of " << n << "\n";
        readPhoto(id, name, date, loc);
        album.addPhoto(id, name, date, loc);
    }

    int choice;
    do {
        cout << "\nCircular Photo Album\n"
             << "1. Add Photo\n2. Insert Photo After Current\n3. Remove Photo by ID\n"
             << "4. Remove Current Photo\n5. Move Next\n6. Move Previous\n"
             << "7. Display Album Forward\n8. Display Album Backward\n"
             << "9. Search Photo\n10. Count Photos\n0. Exit\nChoice: ";
        cin >> choice;

        if (choice == 1 || choice == 2) {
            int id; string name, date, loc;
            readPhoto(id, name, date, loc);
            if (choice == 1) album.addPhoto(id, name, date, loc);
            else album.insertAfterCurrent(id, name, date, loc);
        }
        else if (choice == 3) {
            int id; cout << "Enter ID to remove: "; cin >> id;
            album.removeById(id);
        }
        else if (choice == 4) album.removeCurrent();
        else if (choice == 5) album.moveNext();
        else if (choice == 6) album.movePrevious();
        else if (choice == 7) album.displayForward();
        else if (choice == 8) album.displayBackward();
        else if (choice == 9) {
            int id; cout << "Enter ID to search: "; cin >> id;
            album.searchPhoto(id);
        }
        else if (choice == 10) album.countPhotos();
        else if (choice != 0) cout << "Invalid choice.\n";
    } while (choice != 0 && cin);

    return 0;
}
