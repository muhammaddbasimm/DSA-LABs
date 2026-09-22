#include <iostream>
#include <string>
#include <vector>
using namespace std;

class StringPool {
public:
string** stringPool;
int currentSize;
int maxSize;
vector<string*> leaked;

StringPool() {
maxSize = 5;
currentSize = 0;
stringPool = new string*[maxSize];
for (int i = 0; i < maxSize; i++) stringPool[i] = NULL;
}

void addString(string s) {
if (currentSize < maxSize) {
stringPool[currentSize] = new string(s);
currentSize++;
} else {
cout << "pool is full" << endl;
}
}

void removeString(int index) {
if (index < 0 || index >= currentSize) {
cout << "invalid index" << endl;
return;
}
leaked.push_back(stringPool[index]);
for (int i = index; i < currentSize - 1; i++) stringPool[i] = stringPool[i + 1];
currentSize--;
}

void fixLeaks() {
for (int i = 0; i < leaked.size(); i++) delete leaked[i];
cout << leaked.size() << " leaked strings cleaned up" << endl;
leaked.clear();
}

void showPool() {
cout << "pool contents: ";
for (int i = 0; i < currentSize; i++) cout << *stringPool[i] << " ";
cout << endl;
cout << "current size: " << currentSize << " leaked count: " << leaked.size() << endl;
}
};

int main() {
StringPool pool;
pool.addString("apple");
pool.addString("banana");
pool.addString("cherry");
pool.addString("date");
pool.addString("elderberry");

cout << "after adding 5 strings" << endl;
pool.showPool();

pool.removeString(1);
pool.removeString(0);

cout << "after removing 2 strings without freeing" << endl;
pool.showPool();

pool.fixLeaks();

cout << "after fixing leaks" << endl;
pool.showPool();

return 0;
}
