## B+ Tree Database

This is a sample implementation of a B+ tree database in C++. The B+ tree is a balanced tree data structure commonly used for indexing and organizing large amounts of data in databases. It provides efficient searching, insertion, and deletion operations.

### Usage

To use the B+ tree database, follow these steps:

1. Create an instance of the `BPTree` class:
   ```cpp
   BPTree db;
   ```

2. Insert records into the database using the `insert` method:
   ```cpp
   HashTable record;
   // Set values for the record
   db.insert(primaryKey, &record);
   ```

3. Retrieve records from the database using the `search` method:
   ```cpp
   Node* result = db.search(primaryKey);
   if (result != nullptr) {
       // Record found
       // Access the record through the `result` pointer
   } else {
       // Record not found
   }
   ```

4. Update records in the database using the `update` method:
   ```cpp
   HashTable newRecord;
   // Set new values for the record
   db.update(primaryKey, &newRecord);
   ```

5. Delete records from the database using the `remove` method:
   ```cpp
   db.remove(primaryKey);
   ```

6. Display the records in the database:
   ```cpp
   db.displayLeaves();
   ```

7. Perform other operations as required by your application. The code provides additional methods such as `getNumOfRecords`, `getRecordByProperty`, and `displayOne` that you can use for different purposes.

### Implementation Details

The B+ tree implementation consists of the following classes:

- `BPTree`: Represents the B+ tree database. It provides methods for inserting, searching, updating, removing, and displaying records.
- `Node`: Represents a node in the B+ tree. Each node can be an internal node or a leaf node. The leaf nodes contain the actual records.
- `HashTable`: Represents a hash table used to store key-value pairs. It provides methods for inserting, deleting, and retrieving values based on a key.
- `SinglyLinkedList`: Represents a singly linked list used for collision handling in the hash table. It provides methods for inserting, deleting, and retrieving values.
- `HashNode`: Represents a node in the singly linked list.

The B+ tree uses the `HashTable` class to store records. Each record is associated with a primary key, which is used for indexing and searching purposes. The B+ tree provides efficient access to records based on their primary keys.

Note that this implementation is a simplified version of a B+ tree database and may not include all the features and optimizations found in a production-level database system. It serves as a starting point for understanding the basic concepts and data structures used in a B+ tree database.

### References

- [B+ tree - Wikipedia](https://en.wikipedia.org/wiki/B%2B_tree)
- [Introduction to Database Systems, Chapter 10 - B+ Trees](https://www.db-book.com/db7/slides-dir/PPT-dir/ch10.ppt)
