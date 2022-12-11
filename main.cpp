#include <iostream>
#include <string>
#include <algorithm>

using namespace std;


class HashNode {
public:
    string key;
    string value;
    HashNode *next;


    HashNode() {
        key = "null";
        value = "null";
        next = nullptr;
    }

    HashNode(string key, string value) {
        this->key = key;
        this->value = value;
        this->next = nullptr;
    }
};

class SinglyLinkedList {

private:
    HashNode *head;


    int getPosition(string key) {
        HashNode *temp = head;
        int counter = 1;
        while (temp->next != nullptr) {
            temp = temp->next;
            if (temp->key == key) {
                return counter;
            }
            counter++;
        }
        return counter;
    }

public:
    SinglyLinkedList() { head = nullptr; }

    void clearList() {
        this->head = nullptr;
    }

    void insertNodeAtHead(string key, string value) {
        HashNode *newNode = new HashNode(key, value);
        if (head == nullptr) {
            head = newNode;
            // cout<<head->key<<"\t"<<head->value<<endl;
            return;
        }
        newNode->next = head;
        head = newNode;
        // cout<<head->key<<"\t"<<head->value<<endl;
    }

    void deleteNode(string key) {
        int position = getPosition(key);
        HashNode *temp;
        HashNode *prev;
        temp = head;
        prev = head;
        for (int i = 0; i < position; i++) {
            if (i == 0 && position == 1) {
                head = head->next;
                delete temp;
            } else {
                if (i == position - 1 && temp) {
                    prev->next = temp->next;
                    delete temp;
                } else {
                    prev = temp;
                    if (prev == nullptr)
                        break;
                    temp = temp->next;
                }
            }
        }
    }

    string getValue(string key) {
        HashNode *temp = head;
        while (temp != nullptr) {
            if (temp->key == key) {
                return temp->value;
            }
            temp = temp->next;
        }
        return "null";
    }

    bool containsValue(string value) {
        HashNode *temp = head;
        while (temp != nullptr) {
            if (temp->value == value) {
                return true;
            }
            temp = temp->next;
        }
        return false;
    }


};

class HashTable {
private:
    SinglyLinkedList *values;
    int size;
    string *keys;
    int topOfArr = 0;

    int getHash(string key) {
        key.erase(remove(key.begin(), key.end(), ' '), key.end());
        int hash = 0;
        for (int i = 0; i < key.length(); i++)
            hash = 31 * hash + key.at(i++);
        return hash % size;
    }

public:
    explicit HashTable(int size) {
        this->size = size;
        values = new SinglyLinkedList[this->size];
        keys = new string[size];
        for (int i = 0; i < size; i++) {
            keys[i] = "null";
        }
    }

    HashTable() {}

    int getSize() {
        return size;
    }

    bool contains(string value) {
        for (int i = 0; i < size; i++) {
            if (values[i].containsValue(value)) {
                return true;
            }
        }
        return false;
    }

    void insert(string key, string value) {
        if (this->getValue(key) == "null") {
            this->values[this->getHash(key)].insertNodeAtHead(key, value);
            int hash = this->getHash(key);
            this->keys[topOfArr] = key;
            topOfArr++;
        } else {
            cout << "Cannot insert: This record already exists!" << endl;
        }
    }

    string getValue(string key) {
        return this->values[this->getHash(key)].getValue(key);
    }

    void displayRecord(string key) {
        cout << key << "\t" << this->values[this->getHash(key)].getValue(key) << endl;
    }

    void displayValues() {
        for (int i = 0; i < size; i++) {
            if (this->getValue(this->keys[i]) != "null") {
                cout << this->getValue(this->keys[i]) << "\t";
            }
        }
        cout << endl;
    }

    void deleteValue(string key) {
        this->values[this->getHash(key)].deleteNode(key);
    }
};

// maximum number of keys

class BPTree;


class Node {
    bool IS_LEAF{};
    // arrays of keys and the actual size of this array
    int *key, size{};
    // array of pointers ot the children
    Node **ptr;
    HashTable *hashTable; // its size should be as a size of keys
    int MAX;
    friend class BPTree;
    friend class DBTable;
public:
    Node() {
        this->MAX = 3;
        this->key = new int[this->MAX];
        this->ptr = new Node *[this->MAX + 1];
        this->hashTable = new HashTable[this->MAX];
    }
    Node(int max) {
        MAX = max;
        key = new int[MAX];
        ptr = new Node *[MAX + 1];
        hashTable = new HashTable[MAX];
    }
};
class BPTree {
    Node *root;
    // minimum index (key) of the tree
    int minIdx;
    // maximum index (key) of the tree
    int maxIdx;

    void insertInternal(int primaryKey, Node *current, Node *child);
    void removeInternal(int x, Node *current, Node *child);
    Node *findParent(Node *current, Node *child);
    int updateMinMax(int prevIdx, bool forMinIdx);
public:
    BPTree() {
        root = nullptr;
        minIdx = -1;
        maxIdx = -1;
    }

    void insert(int primaryKey, HashTable *hashTable);
    void remove(int primaryKey);
    void display(Node *current);

    int getNumOfRecords() {
        Node *leastValueNode = this->search(this->minIdx);
        Node *current = leastValueNode;
        int currentKey = -1;
        int count = 0;
        while (currentKey < this->maxIdx) {
            for (int i = 0; i < current->size; i++) {
                currentKey = current->key[i];
            }
            current = current->ptr[current->size];
            count++;
        }
        return count;
    }

    Node *search(int searchedKey) {
        if (root == nullptr) {
            cout << "Tree is empty\n";
        } else {
            Node *current = root;
            while (!current->IS_LEAF) {
                for (int i = 0; i < current->size; i++) {
                    if (searchedKey < current->key[i]) {
                        current = current->ptr[i];
                        break;
                    }
                    if (i == current->size - 1) {
                        current = current->ptr[i + 1];
                        break;
                    }
                }
            }
            for (int i = 0; i < current->size; i++) {
                if (current->key[i] == searchedKey) {
                    return current;
                }
            }
            cout << "Not found\n";
        }
        return nullptr;
    }

    void update(int primaryKey, HashTable *hashTable) {
        Node *beingUpd = this->search(primaryKey);
        if (beingUpd != nullptr) {
            for (int i = 0; i < beingUpd->size; i++) {
                if (beingUpd->key[i] == primaryKey) {
                    beingUpd->hashTable[i] = *hashTable;
                }
            }
        } else {
            cout << "Not found!" << endl;
        }

    }

    void getRecordByProperty(string property) {
        Node *leastValueNode = this->search(this->minIdx);
        Node *current = leastValueNode;
        int currentKey = -1;
        bool found = false;
        while (currentKey < maxIdx) {
            for (int i = 0; i < current->size; i++) {
                currentKey = current->key[i];
                if (current->hashTable[i].contains(property)) {
                    found = true;
                    current->hashTable[i].displayValues();
                }
            }
            current = current->ptr[current->size];
        }
        if(!found){
            cout<<"Not found!"<<endl;
        }
    }

    void displayLeaves() {
        Node *leastValueNode = this->search(this->minIdx);
        Node *current = leastValueNode;
        int currentKey = -1;
        while (currentKey < maxIdx) {
            for (int i = 0; i < current->size; i++) {
                currentKey = current->key[i];
                cout << current->key[i] << " ";
                current->hashTable[i].displayValues();
            }
            current = current->ptr[current->size];
        }
    }

    void displayOne(int id) {
        Node *leastValueNode = this->search(this->minIdx);
        Node *current = leastValueNode;
        int currentKey = -1;
        bool found = false;
        while (currentKey < maxIdx) {
            for (int i = 0; i < current->size; i++) {
                currentKey = current->key[i];
                if (current->key[i] == id) {
                    found = true;
                    current->hashTable[i].displayValues();
                }
            }
            current = current->ptr[current->size];
        }
        if (!found) {
            cout << "Not Found!" << endl;
        }
    }

    Node *getRoot() {
        return root;
    }
};

void BPTree::insert(int primaryKey, HashTable *hashTable)  {
    // if the being inserted primary key (index, key) is smaller or equal to the minimum index of the whole tree
    // then assign this value to the minimum index
    // almost the same logic applies to the maximum index
    // they are needed for the sequential traversal of leaves
    if (this->minIdx == -1 || this->search(primaryKey) == nullptr) {
        if (this->minIdx == -1 && this->maxIdx == -1) {
            this->minIdx = primaryKey;
            this->maxIdx = primaryKey;
        } else if (primaryKey <= this->minIdx) {
            this->minIdx = primaryKey;
        } else if (primaryKey >= this->maxIdx) {
            this->maxIdx = primaryKey;
        }
        if (root == nullptr) {
            root = new Node;
            root->key[0] = primaryKey;
            root->IS_LEAF = true;
            root->hashTable[0] = *hashTable;
            root->size = 1;
        } else {
            Node *current = root;
            Node *parent;
            while (!current->IS_LEAF) {
                parent = current;
                for (int i = 0; i < current->size; i++) {
                    if (primaryKey < current->key[i]) {
                        current = current->ptr[i];
                        break;
                    }
                    if (i == current->size - 1) {
                        current = current->ptr[i + 1];
                        break;
                    }
                }
            }
            if (current->size < current->MAX) {
                int i = 0;
                while (primaryKey > current->key[i] && i < current->size)
                    i++;
                for (int j = current->size; j > i; j--) {
                    current->key[j] = current->key[j - 1];
                }
                current->key[i] = primaryKey;
                current->hashTable[i] = *hashTable;
                current->size++;
                current->ptr[current->size] = current->ptr[current->size - 1];
                current->ptr[current->size - 1] = nullptr;
            } else {
                Node *newLeaf = new Node;
                Node *virtualNode = new Node(current->MAX + 1);
                for (int i = 0; i < current->MAX; i++) {
                    virtualNode->key[i] = current->key[i];
                    virtualNode->hashTable[i] = current->hashTable[i];
                }
                int i = 0, j;
                while (primaryKey > virtualNode->key[i] && i < current->MAX)
                    i++;
                for (int j = current->MAX + 1; j > i; j--) {
                    virtualNode->key[j] = virtualNode->key[j - 1];
                    virtualNode->hashTable[j] = virtualNode->hashTable[j - 1];
                }
                virtualNode->key[i] = primaryKey;
                virtualNode->hashTable[i] = *hashTable;
                newLeaf->IS_LEAF = true;
                current->size = (current->MAX + 1) / 2;
                newLeaf->size = (current->MAX + 1) - current->size;
                // link the leaf nodes through linked list (current is linked to the new leaf)
                current->ptr[current->size] = newLeaf;
                newLeaf->ptr[newLeaf->size] = current->ptr[current->MAX];
                current->ptr[current->MAX] = nullptr;
                // assigning values of 2 leaf nodes to the values of parts of the split node (virtualNode)
                for (i = 0; i < current->size; i++) {
                    current->key[i] = virtualNode->key[i];
                    current->hashTable[i] = virtualNode->hashTable[i];

                }
                for (i = 0, j = current->size; i < newLeaf->size; i++, j++) {
                    newLeaf->key[i] = virtualNode->key[j];
                    newLeaf->hashTable[i] = virtualNode->hashTable[j];
                }
                if (current == root) {
                    Node *newRoot = new Node;
                    newRoot->key[0] = newLeaf->key[0];
                    newRoot->ptr[0] = current;
                    newRoot->ptr[1] = newLeaf;
                    newRoot->IS_LEAF = false;
                    newRoot->size = 1;
                    root = newRoot;
                } else {
                    // right biased
                    insertInternal(newLeaf->key[0], parent, newLeaf);
                }
            }
        }
    } else {
        cout << "This primary key already exists!" << endl;
    }
}
void BPTree::remove(int primaryKey) {
    if (minIdx == primaryKey) {
        this->minIdx = this->updateMinMax(minIdx, true);
    } else if (maxIdx == primaryKey) {
        this->maxIdx = this->updateMinMax(maxIdx, false);
    }

    if (root == nullptr) {
        cout << "Tree empty\n";
    } else {
        Node *current = root;
        Node *parent;
        int leftSibling, rightSibling;
        while (!current->IS_LEAF) {
            for (int i = 0; i < current->size; i++) {
                parent = current;
                leftSibling = i - 1;
                rightSibling = i + 1;
                if (primaryKey < current->key[i]) {
                    current = current->ptr[i];
                    break;
                }
                if (i == current->size - 1) {
                    leftSibling = i;
                    rightSibling = i + 2;
                    current = current->ptr[i + 1];
                    break;
                }
            }
        }
        bool found = false;
        int pos;
        for (pos = 0; pos < current->size; pos++) {
            if (current->key[pos] == primaryKey) {
                found = true;
                break;
            }
        }
        if (!found) {
            cout << "Not found\n";
            return;
        }
        for (int i = pos; i < current->size; i++) {
            current->key[i] = current->key[i + 1];
            current->hashTable[i] = current->hashTable[i + 1];
        }
        current->size--;
        if (current == root) {
            for (int i = 0; i < current->MAX + 1; i++) {
                current->ptr[i] = nullptr;
            }
            if (current->size == 0) {
                cout << "Tree died\n";
                delete[] current->key;
                delete[] current->ptr;
                delete[] current->hashTable;
                delete current;
                root = nullptr;
            }
            return;
        }
        current->ptr[current->size] = current->ptr[current->size + 1];
        current->ptr[current->size + 1] = nullptr;
        if (current->size >= (current->MAX + 1) / 2) {
            return;
        }
        if (leftSibling >= 0) {
            Node *leftNode = parent->ptr[leftSibling];
            if (leftNode->size >= (current->MAX + 1) / 2 + 1) {
                for (int i = current->size; i > 0; i--) {
                    current->key[i] = current->key[i - 1];
                    current->hashTable[i] = current->hashTable[i - 1];
                }
                current->size++;
                current->ptr[current->size] = current->ptr[current->size - 1];
                current->ptr[current->size - 1] = nullptr;
                current->key[0] = leftNode->key[leftNode->size - 1];
                current->hashTable[0] = leftNode->hashTable[leftNode->size - 1];
                leftNode->size--;
                leftNode->ptr[leftNode->size] = current;
                leftNode->ptr[leftNode->size + 1] = nullptr;
                parent->key[leftSibling] = current->key[0];
                return;
            }
        }
        if (rightSibling <= parent->size) {
            Node *rightNode = parent->ptr[rightSibling];
            if (rightNode->size >= (current->MAX + 1) / 2 + 1) {
                current->size++;
                current->ptr[current->size] = current->ptr[current->size - 1];
                current->ptr[current->size - 1] = nullptr;
                current->key[current->size - 1] = rightNode->key[0];
                current->hashTable[current->size - 1] = rightNode->hashTable[0];
                rightNode->size--;
                rightNode->ptr[rightNode->size] = rightNode->ptr[rightNode->size + 1];
                rightNode->ptr[rightNode->size + 1] = nullptr;
                for (int i = 0; i < rightNode->size; i++) {
                    rightNode->key[i] = rightNode->key[i + 1];
                    rightNode->hashTable[i] = rightNode->hashTable[i + 1];
                }
                parent->key[rightSibling - 1] = rightNode->key[0];
                return;
            }
        }
        if (leftSibling >= 0) {
            Node *leftNode = parent->ptr[leftSibling];
            for (int i = leftNode->size, j = 0; j < current->size; i++, j++) {
                leftNode->key[i] = current->key[j];
                leftNode->hashTable[i] = current->hashTable[j];
            }
            leftNode->ptr[leftNode->size] = nullptr;
            leftNode->size += current->size;
            leftNode->ptr[leftNode->size] = current->ptr[current->size];
            removeInternal(parent->key[leftSibling], parent, current);
            delete[] current->key;
            delete[] current->ptr;
            delete[] current->hashTable;
            delete current;
        } else if (rightSibling <= parent->size) {
            Node *rightNode = parent->ptr[rightSibling];
            for (int i = current->size, j = 0; j < rightNode->size; i++, j++) {
                current->key[i] = rightNode->key[j];
                current->hashTable[i] = rightNode->hashTable[j];
            }
            current->ptr[current->size] = nullptr;
            current->size += rightNode->size;
            current->ptr[current->size] = rightNode->ptr[rightNode->size];
            cout << "Merging two leaf nodes\n";
            removeInternal(parent->key[rightSibling - 1], parent, rightNode);
            delete[] rightNode->key;
            delete[] rightNode->ptr;
            delete[] rightNode->hashTable;
            delete rightNode;
        }
    }
}
void BPTree::display(Node *current) {
    if (current != nullptr) {
        for (int i = 0; i < current->size; i++) {
            cout << current->key[i] << " ";
        }
        cout << "\n";
        if (!current->IS_LEAF) {
            for (int i = 0; i < current->size + 1; i++) {
                display(current->ptr[i]);
            }
        }
    }
}
void BPTree::insertInternal(int primaryKey, Node *current, Node *child) {
    if (current->size < current->MAX) {
        int i = 0;
        while (primaryKey > current->key[i] && i < current->size)
            i++;
        for (int j = current->size; j > i; j--) {
            current->key[j] = current->key[j - 1];
        }
        for (int j = current->size + 1; j > i + 1; j--) {
            current->ptr[j] = current->ptr[j - 1];
        }
        current->key[i] = primaryKey;
        current->size++;
        current->ptr[i + 1] = child;
    } else {
        Node *newInternal = new Node;
        int virtualKey[current->MAX + 1];
        Node *virtualPtr[current->MAX - +2];
        for (int i = 0; i < current->MAX; i++) {
            virtualKey[i] = current->key[i];
        }
        for (int i = 0; i < current->MAX + 1; i++) {
            virtualPtr[i] = current->ptr[i];
        }
        int i = 0, j;
        while (primaryKey > virtualKey[i] && i < current->MAX)
            i++;
        for (int j = current->MAX + 1; j > i; j--) {
            virtualKey[j] = virtualKey[j - 1];
        }
        virtualKey[i] = primaryKey;
        for (int j = current->MAX + 2; j > i + 1; j--) {
            virtualPtr[j] = virtualPtr[j - 1];
        }
        virtualPtr[i + 1] = child;
        newInternal->IS_LEAF = false;
        current->size = (current->MAX + 1) / 2;
        newInternal->size = current->MAX - (current->MAX + 1) / 2;
        for (i = 0, j = current->size + 1; i < newInternal->size; i++, j++) {
            newInternal->key[i] = virtualKey[j];
        }
        for (i = 0, j = current->size + 1; i < newInternal->size + 1; i++, j++) {
            newInternal->ptr[i] = virtualPtr[j];
        }
        if (current == root) {
            Node *newRoot = new Node;
            newRoot->key[0] = current->key[current->size];
            newRoot->ptr[0] = current;
            newRoot->ptr[1] = newInternal;
            newRoot->IS_LEAF = false;
            newRoot->size = 1;
            root = newRoot;
        } else {
            insertInternal(current->key[current->size], findParent(root, current), newInternal);
        }
    }
}
void BPTree::removeInternal(int x, Node *current, Node *child) {
    if (current == root) {
        if (current->size == 1) {
            if (current->ptr[1] == child) {
                delete[] child->key;
                delete[] child->ptr;
                delete[] child->hashTable;
                delete child;
                root = current->ptr[0];
                delete[] current->key;
                delete[] current->ptr;
                delete[] current->hashTable;
                delete current;
                cout << "Changed root node\n";
                return;
            } else if (current->ptr[0] == child) {
                delete[] child->key;
                delete[] child->ptr;
                delete[] child->hashTable;
                delete child;
                root = current->ptr[1];
                delete[] current->key;
                delete[] current->ptr;
                delete[] current->hashTable;
                delete current;
                cout << "Changed root node\n";
                return;
            }
        }
    }
    int pos;
    for (pos = 0; pos < current->size; pos++) {
        if (current->key[pos] == x) {
            break;
        }
    }
    for (int i = pos; i < current->size; i++) {
        current->key[i] = current->key[i + 1];
    }
    for (pos = 0; pos < current->size + 1; pos++) {
        if (current->ptr[pos] == child) {
            break;
        }
    }
    for (int i = pos; i < current->size + 1; i++) {
        current->ptr[i] = current->ptr[i + 1];
    }
    current->size--;
    if (current->size >= (current->MAX + 1) / 2 - 1) {
        return;
    }
    if (current == root)
        return;
    Node *parent = findParent(root, current);
    int leftSibling, rightSibling;
    for (pos = 0; pos < parent->size + 1; pos++) {
        if (parent->ptr[pos] == current) {
            leftSibling = pos - 1;
            rightSibling = pos + 1;
            break;
        }
    }
    if (leftSibling >= 0) {
        Node *leftNode = parent->ptr[leftSibling];
        if (leftNode->size >= (current->MAX + 1) / 2) {
            for (int i = current->size; i > 0; i--) {
                current->key[i] = current->key[i - 1];
            }
            current->key[0] = parent->key[leftSibling];
            parent->key[leftSibling] = leftNode->key[leftNode->size - 1];
            for (int i = current->size + 1; i > 0; i--) {
                current->ptr[i] = current->ptr[i - 1];
            }
            current->ptr[0] = leftNode->ptr[leftNode->size];
            current->size++;
            leftNode->size--;
            return;
        }
    }
    if (rightSibling <= parent->size) {
        Node *rightNode = parent->ptr[rightSibling];
        if (rightNode->size >= (current->MAX + 1) / 2) {
            current->key[current->size] = parent->key[pos];
            parent->key[pos] = rightNode->key[0];
            for (int i = 0; i < rightNode->size - 1; i++) {
                rightNode->key[i] = rightNode->key[i + 1];
            }
            current->ptr[current->size + 1] = rightNode->ptr[0];
            for (int i = 0; i < rightNode->size; ++i) {
                rightNode->ptr[i] = rightNode->ptr[i + 1];
            }
            current->size++;
            rightNode->size--;
            return;
        }
    }
    if (leftSibling >= 0) {
        Node *leftNode = parent->ptr[leftSibling];
        leftNode->key[leftNode->size] = parent->key[leftSibling];
        for (int i = leftNode->size + 1, j = 0; j < current->size; j++) {
            leftNode->key[i] = current->key[j];
        }
        for (int i = leftNode->size + 1, j = 0; j < current->size + 1; j++) {
            leftNode->ptr[i] = current->ptr[j];
            current->ptr[j] = nullptr;
        }
        leftNode->size += current->size + 1;
        current->size = 0;
        removeInternal(parent->key[leftSibling], parent, current);
    } else if (rightSibling <= parent->size) {
        Node *rightNode = parent->ptr[rightSibling];
        current->key[current->size] = parent->key[rightSibling - 1];
        for (int i = current->size + 1, j = 0; j < rightNode->size; j++) {
            current->key[i] = rightNode->key[j];
        }
        for (int i = current->size + 1, j = 0; j < rightNode->size + 1; j++) {
            current->ptr[i] = rightNode->ptr[j];
            rightNode->ptr[j] = nullptr;
        }
        current->size += rightNode->size + 1;
        rightNode->size = 0;
        removeInternal(parent->key[rightSibling - 1], parent, rightNode);
    }
}
Node* BPTree::findParent(Node *current, Node *child) {
    Node *parent;
    if (current->IS_LEAF || (current->ptr[0])->IS_LEAF) {
        return nullptr;
    }
    for (int i = 0; i < current->size + 1; i++) {
        if (current->ptr[i] == child) {
            parent = current;
            return parent;
        } else {
            parent = findParent(current->ptr[i], child);
            if (parent != nullptr)
                return parent;
        }
    }
    return parent;
}
int BPTree::updateMinMax(int prevIdx, bool forMinIdx) {
    Node *leastValueNode = this->search(this->minIdx);
    Node *current = leastValueNode;
    int currentKey = -1;
    int count = 0;
    while (currentKey < this->maxIdx) {
        for (int i = 0; i < current->size; i++) {
            if (forMinIdx) {
                if (current->key[i] > prevIdx)
                    return current->key[i];
            } else {
                if (count == (this->getNumOfRecords() - 1)) {
                    if (i == (current->size - 2)) {
                        return current->key[i];
                    }
                }
            }
        }
        current = current->ptr[current->size];
        count++;
    }
}

//class Database;
class DBTable {
private:

    // find out how to link through linked list the leaf nodes
    // it is needed in order to traverse through the whole table and display it
    // check the hashtable connection with b+ tree (does it work fine or not)
    string name;
    string *columns;
    int size;

    friend class Database;

public:
    BPTree bpTree;

    DBTable(string name, string columns[], int size) {
        this->name = name;
        this->columns = columns;
        this->size = size;
    }

    DBTable() {}

    void addRow(int id, string values[]) {
        HashTable *hashTable = new HashTable(size);
        string value;
        for (int i = 0; i < size; i++) {
            hashTable->insert(columns[i], values[i]);
        }
        bpTree.insert(id, hashTable);
    }

    void deleteRowById(int id) {
        bpTree.remove(id);
    }

    void display() {
        cout << "ID" << "\t";
        for (int i = 0; i < size; i++) {
            cout << columns[i] << "\t";
        }
        cout << endl << "- - - - - - - - - - - - - - - - - - - - -";
        cout << endl;
        bpTree.displayLeaves();
    }

    void displayOne(int id) {
        this->bpTree.displayOne(id);
    }

    void displayByProperty(string property){
        this->bpTree.getRecordByProperty(property);
    }

//    void displayOne(int id){
//        bpTree.getSpecificRecord(id).displayValues();
//    }

//    void displayOneByProperty(string key, string value){
//        bpTree.getRecordByProperty(key).displayValues();
//    }


    void updateRowById(int id, string values[]) {
        HashTable *hashTable = new HashTable(size);
        string value;
        for (int i = 0; i < size; i++) {
            hashTable->insert(columns[i], values[i]);
        }
        this->bpTree.update(id, hashTable);
    }


};
//
//class Database {
//    DBTable **tables;
//    int size;
//    int topOfArr;
//
//public:
//    Database(int size) {
//        tables = new DBTable *[size];
//        topOfArr = 0;
//    }
//
//    void addTable(string name, string columns[], int size){
//        DBTable table(name, columns, size);
//        tables[topOfArr] = &table;
//        topOfArr++;
//    }
//
//    void deleteTable(string name){
//        for(int i = 0; i<=topOfArr; i++){
//            if(tables[i]->name == name){
//                tables[i] = nullptr;
//            }
//        }
//    }
//
//    DBTable* getTable(string name){
//        for(int i = 0; i<=topOfArr; i++){
//            if(tables[i]->name == name){
//                return tables[i];
//            }
//        }
//    }
//
//    void display(){
//        for(int i = 0; i<=topOfArr; i++){
//           cout<<tables[i]->name<<"\t";
//        }
//    }
//
//};

int main() {
//    HashTable table(10);
//    table.insert("10", "rat1");
//    table.insert("11", "rat2");
//    table.insert("12", "absolute");

//    table.displayRecord("10");
//    table.displayRecord("11");
//    table.displayRecord("12");
//    table.displayValues();

//    BPTree node;
//    node.insert(5);
//    node.insert(15);
//    node.insert(25);
//    node.insert(35);
//    node.insert(45);
//    node.insert(55);
//    node.insert(40);
//    node.insert(30);
//    node.insert(20);
//    node.display(node.getRoot());
//    node.displayLeaves();

    string columns[2] = {"Name", "Surname"};
    string query1[2] = {"Komiljon", "Yuldashev"};
    string query2[2] = {"Ravshan", "Ubaydullayev"};
    string query3[2] = {"Shokhrukh", "Yakhyoev"};
    string query4[2] = {"Ezoza", "Yuldasheva"};
    string query5[2] = {"Nuriddin", "Yuldashev"};
    string query6[2] = {"Abdurauf", "Vakhobov"};
    string query7[2] = {"Farrukh", "Utkurov"};
    string query8[2] = {"Nozima", "Ergasheva"};
    string query9[2] = {"Nigina", "Ganieva"};
    string updQuery[2] = {"Johnny", "Koshev"};
    DBTable table1("identification", columns, 2);
    table1.addRow(5, query1); // 1
    table1.addRow(15, query2); // 2
    table1.addRow(25, query3); // 3
    table1.addRow(35, query4); // 4
    table1.addRow(45, query5); // 5
    table1.addRow(55, query6); // 6
    table1.addRow(40, query7); // 7
    table1.addRow(30, query8);// 8
    table1.addRow(20, query9); // 9
    table1.deleteRowById(25);
    table1.deleteRowById(20);
    table1.deleteRowById(45);
    table1.updateRowById(5, updQuery);
    table1.display();
    cout << endl;
    table1.displayOne(55);
    cout << endl;
    table1.displayByProperty("Farrukh");


//
//    Database db1(5);
//
//    db1.addTable("identification", columns, 2);
//    db1.getTable("identification")->addRow(5, query1); // 1
//    db1.getTable("identification")->addRow(15, query2); // 2
//    db1.getTable("identification")->addRow(25, query3); // 3
//    db1.getTable("identification")->addRow(35, query4); // 4
//    db1.getTable("identification")->addRow(45, query5); // 5
//    db1.getTable("identification")->addRow(55, query6); // 6
//    db1.getTable("identification")->addRow(40, query7); // 7
//    db1.getTable("identification")->addRow(30, query8);// 8
//    db1.getTable("identification")->addRow(20, query9); // 9
//    db1.getTable("identification")->deleteRowById(25);
//    db1.getTable("identification")->deleteRowById(20);
//    db1.getTable("identification")->deleteRowById(45);
//    db1.getTable("identification")->updateRowById(5, updQuery);
//    db1.getTable("identification")->display();
    return 0;
}