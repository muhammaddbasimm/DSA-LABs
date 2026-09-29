#include <iostream>
#include <string>
#include <limits>
#include <utility>
using namespace std;

struct Song {
  int id;
  string name;
  int minutes;
  int seconds;
  Song* prev;
  Song* next;
  Song(int i, string n, int m, int s) : id(i), name(n), minutes(m), seconds(s), prev(nullptr), next(nullptr) {}
};

class Playlist {
  Song* head;
  Song* tail;
  Song* current;

  void printSong(Song* s) {
    cout << "ID: " << s->id << " | " << s->name << " | " << s->minutes << ":";
    if (s->seconds < 10) cout << "0";
    cout << s->seconds << endl;
  }

public:
  Playlist() : head(nullptr), tail(nullptr), current(nullptr) {}

  ~Playlist() {
    while (head != nullptr) {
      Song* temp = head;
      head = head->next;
      delete temp;
    }
  }

  Song* find(int id) {
    for (Song* p = head; p != nullptr; p = p->next)
      if (p->id == id) return p;
    return nullptr;
  }

  bool addSong(int id, string name, int m, int s) {
    if (find(id) != nullptr) return false;
    Song* node = new Song(id, name, m, s);
    if (tail == nullptr) {
      head = tail = node;
    } else {
      tail->next = node;
      node->prev = tail;
      tail = node;
    }
    return true;
  }

  bool deleteSong(int id) {
    Song* node = find(id);
    if (node == nullptr) return false;
    if (node->prev != nullptr) node->prev->next = node->next;
    else head = node->next;
    if (node->next != nullptr) node->next->prev = node->prev;
    else tail = node->prev;
    if (current == node) current = (node->next != nullptr) ? node->next : node->prev;
    delete node;
    return true;
  }

  void displayForward() {
    if (head == nullptr) {
      cout << "Playlist is empty." << endl;
      return;
    }
    for (Song* p = head; p != nullptr; p = p->next) printSong(p);
  }

  void displayBackward() {
    if (tail == nullptr) {
      cout << "Playlist is empty." << endl;
      return;
    }
    for (Song* p = tail; p != nullptr; p = p->prev) printSong(p);
  }

  void searchSong(int id) {
    Song* s = find(id);
    if (s == nullptr) {
      cout << "Song not found." << endl;
    } else {
      cout << "Found -> ";
      printSong(s);
    }
  }

  void playNext() {
    if (head == nullptr) {
      cout << "Playlist is empty." << endl;
      return;
    }
    if (current == nullptr) current = head;
    else if (current->next != nullptr) current = current->next;
    else cout << "Already at the last song." << endl;
    cout << "Now playing -> ";
    printSong(current);
  }

  void playPrevious() {
    if (head == nullptr) {
      cout << "Playlist is empty." << endl;
      return;
    }
    if (current == nullptr) current = head;
    else if (current->prev != nullptr) current = current->prev;
    else cout << "Already at the first song." << endl;
    cout << "Now playing -> ";
    printSong(current);
  }

  void reverse() {
    Song* p = head;
    while (p != nullptr) {
      swap(p->prev, p->next);
      p = p->prev;
    }
    swap(head, tail);
    cout << "Playlist reversed." << endl;
  }
};

int readInt(string prompt) {
  int x;
  cout << prompt;
  while (!(cin >> x)) {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Invalid input. " << prompt;
  }
  cin.ignore(numeric_limits<streamsize>::max(), '\n');
  return x;
}

int main() {
  Playlist list;
  int choice;

  do {
    cout << "\n1. Add Song\n2. Delete Song\n3. Display Forward\n4. Display Backward\n";
    cout << "5. Search Song\n6. Play Next\n7. Play Previous\n8. Reverse Playlist\n0. Exit\n";
    choice = readInt("Enter choice: ");

    if (choice == 1) {
      int id = readInt("Song ID: ");
      string name;
      cout << "Song Name: ";
      getline(cin, name);
      int m = readInt("Minutes: ");
      int s = readInt("Seconds: ");
      if (m < 0 || s < 0 || s > 59) cout << "Invalid duration." << endl;
      else if (list.addSong(id, name, m, s)) cout << "Song added." << endl;
      else cout << "ID already exists." << endl;
    }
    else if (choice == 2) {
      int id = readInt("Song ID to delete: ");
      if (list.deleteSong(id)) cout << "Song deleted." << endl;
      else cout << "Song not found." << endl;
    }
    else if (choice == 3) list.displayForward();
    else if (choice == 4) list.displayBackward();
    else if (choice == 5) list.searchSong(readInt("Song ID to search: "));
    else if (choice == 6) list.playNext();
    else if (choice == 7) list.playPrevious();
    else if (choice == 8) list.reverse();
    else if (choice != 0) cout << "Invalid choice." << endl;
  } while (choice != 0);

  return 0;
}
