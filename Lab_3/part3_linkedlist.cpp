#include <iostream>
using namespace std;

struct Node {
int data;
Node* next;
};

Node* head = NULL;

void insertAtHead(int value) {
Node* newNode = new Node();
newNode->data = value;
newNode->next = head;
head = newNode;
}

void insertAtThird(int value) {
Node* newNode = new Node();
newNode->data = value;

if (head == NULL || head->next == NULL) {
cout << "list has fewer than 2 nodes, inserting at head instead" << endl;
newNode->next = head;
head = newNode;
return;
}

Node* temp = head;
for (int i = 0; i < 1; i++) temp = temp->next;
newNode->next = temp->next;
temp->next = newNode;
}

void displayList() {
Node* temp = head;
while (temp != NULL) {
cout << temp->data << " -> ";
temp = temp->next;
}
cout << "NULL" << endl;
}

void deleteLast() {
if (head == NULL) {
cout << "list is empty" << endl;
return;
}
if (head->next == NULL) {
delete head;
head = NULL;
return;
}
Node* temp = head;
while (temp->next->next != NULL) temp = temp->next;
delete temp->next;
temp->next = NULL;
}

int countNodes() {
int count = 0;
Node* temp = head;
while (temp != NULL) {
count++;
temp = temp->next;
}
return count;
}

void reverseList() {
Node* prev = NULL;
Node* current = head;
Node* next = NULL;
while (current != NULL) {
next = current->next;
current->next = prev;
prev = current;
current = next;
}
head = prev;
}

void searchValue(int value) {
Node* temp = head;
int index = 0;
while (temp != NULL) {
if (temp->data == value) {
cout << "found at position " << index << endl;
return;
}
temp = temp->next;
index++;
}
cout << "value not found" << endl;
}

int main() {
int choice, value;

while (true) {
cout << "\n1. Insert at head" << endl;
cout << "2. Insert at 3rd position" << endl;
cout << "3. Display list" << endl;
cout << "4. Delete last node" << endl;
cout << "5. Count nodes" << endl;
cout << "6. Reverse list" << endl;
cout << "7. Search value" << endl;
cout << "8. Exit" << endl;
cout << "enter choice: ";
cin >> choice;

if (choice == 1) {
cout << "enter value: ";
cin >> value;
insertAtHead(value);
} else if (choice == 2) {
cout << "enter value: ";
cin >> value;
insertAtThird(value);
} else if (choice == 3) {
displayList();
} else if (choice == 4) {
deleteLast();
displayList();
} else if (choice == 5) {
cout << "node count: " << countNodes() << endl;
} else if (choice == 6) {
reverseList();
displayList();
} else if (choice == 7) {
cout << "enter value to search: ";
cin >> value;
searchValue(value);
} else if (choice == 8) {
break;
} else {
cout << "invalid choice" << endl;
}
}

return 0;
}
