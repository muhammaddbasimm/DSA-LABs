#include <iostream>
#include <string>
#include <limits>
using namespace std;

struct Tab {
    int id;
    string title;
    string url;
    Tab* next;
    Tab* prev;
};

class TabManager {
private:
    Tab* current;

public:
    TabManager() : current(nullptr) {}

    ~TabManager() {
        while (current != nullptr) closeCurrentTab(false);
    }

    bool isEmpty() const { return current == nullptr; }

    Tab* find(int id) const {
        if (current == nullptr) return nullptr;
        Tab* temp = current;
        do {
            if (temp->id == id) return temp;
            temp = temp->next;
        } while (temp != current);
        return nullptr;
    }

    void openNewTab(int id, const string& title, const string& url) {
        if (find(id) != nullptr) {
            cout << "A tab with ID " << id << " already exists.\n";
            return;
        }
        Tab* node = new Tab{id, title, url, nullptr, nullptr};

        if (current == nullptr) {
            node->next = node;
            node->prev = node;
            current = node;
        } else {
            node->prev = current;
            node->next = current->next;
            current->next->prev = node;
            current->next = node;
        }
        cout << "Opened tab " << id << ".\n";
    }

    void closeCurrentTab(bool verbose = true) {
        if (current == nullptr) {
            if (verbose) cout << "No tabs open.\n";
            return;
        }
        Tab* toDelete = current;
        if (current->next == current) {
            current = nullptr;
        } else {
            current->prev->next = current->next;
            current->next->prev = current->prev;
            current = current->next;
        }
        if (verbose) cout << "Closed tab " << toDelete->id << ".\n";
        delete toDelete;
    }

    void moveNext() {
        if (current == nullptr) { cout << "No tabs open.\n"; return; }
        current = current->next;
        displayCurrent();
    }

    void movePrevious() {
        if (current == nullptr) { cout << "No tabs open.\n"; return; }
        current = current->prev;
        displayCurrent();
    }

    void displayCurrent() const {
        if (current == nullptr) { cout << "No tabs open.\n"; return; }
        cout << "[Current] ID: " << current->id
             << " | Title: " << current->title
             << " | URL: " << current->url << "\n";
    }

    void displayForward() const {
        if (current == nullptr) { cout << "No tabs open.\n"; return; }
        Tab* temp = current;
        do {
            cout << "ID: " << temp->id << " | " << temp->title << " | " << temp->url << "\n";
            temp = temp->next;
        } while (temp != current);
    }

    void displayBackward() const {
        if (current == nullptr) { cout << "No tabs open.\n"; return; }
        Tab* temp = current;
        do {
            cout << "ID: " << temp->id << " | " << temp->title << " | " << temp->url << "\n";
            temp = temp->prev;
        } while (temp != current);
    }

    void searchTab(int id) const {
        Tab* t = find(id);
        if (t == nullptr) cout << "Tab " << id << " not found.\n";
        else cout << "Found -> ID: " << t->id << " | Title: " << t->title
                  << " | URL: " << t->url << "\n";
    }
};

int main() {
    TabManager tm;
    int choice;

    do {
        cout << "\nBrowser Tab Manager\n"
             << "1. Open New Tab\n2. Close Current Tab\n3. Move Next\n4. Move Previous\n"
             << "5. Display Current Tab\n6. Display All Tabs Forward\n7. Display All Tabs Backward\n"
             << "8. Search Tab\n0. Exit\nChoice: ";
        cin >> choice;

        if (choice == 1) {
            int id; string title, url;
            cout << "Tab ID: "; cin >> id;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Website title: "; getline(cin, title);
            cout << "URL: "; getline(cin, url);
            tm.openNewTab(id, title, url);
        }
        else if (choice == 2) tm.closeCurrentTab();
        else if (choice == 3) tm.moveNext();
        else if (choice == 4) tm.movePrevious();
        else if (choice == 5) tm.displayCurrent();
        else if (choice == 6) tm.displayForward();
        else if (choice == 7) tm.displayBackward();
        else if (choice == 8) {
            int id; cout << "Enter ID to search: "; cin >> id;
            tm.searchTab(id);
        }
        else if (choice != 0) cout << "Invalid choice.\n";
    } while (choice != 0 && cin);

    return 0;
}
