#include <iostream>
#include <vector>
#include <list>
#include <cmath>
#include <string>
#include <algorithm> // For std::find

using namespace std;

// 1. Hashing Techniques

// Division Method
int divisionMethod(int key, int tableSize) {
    return key % tableSize;
}

// Multiplication Method
int multiplicationMethod(int key, int tableSize) {
    double A = 0.6180339887;  // Golden ratio
    return int(tableSize * (key * A - int(key * A)));
}

// Mid Square Method
int midSquareMethod(int key, int tableSize) {
    int r = log10(tableSize) + 1;  // number of digits to extract (r digits)
    int square = key * key;

    // Convert square to string to easily extract middle digits
    string squareStr = to_string(square);

    // Ensure that we extract the middle r digits
    int startIdx = (squareStr.length() - r) / 2;
    string middleDigitsStr = squareStr.substr(startIdx, r);

    // Convert middle digits back to an integer
    int middleDigits = stoi(middleDigitsStr);

    return middleDigits % tableSize;  // Return the hash value modulo tableSize
}

// Folding Method
int foldingMethod(int key, int tableSize) {
    int addressSpace;
    cout << "Enter the number of digits for address space (e.g., 2): ";
    cin >> addressSpace;

    int sum = 0;

    // Split the key into parts based on the address space
    while (key > 0) {
        int part = key % static_cast<int>(pow(10, addressSpace)); // Extract the last 'addressSpace' digits
        sum += part;
        key /= static_cast<int>(pow(10, addressSpace));          // Remove the extracted digits from the key
    }

    // Return the hash value after modulo operation
    return sum % tableSize;
}

// Function to display the result of each hashing technique
void displayHash(int key, int tableSize, int hashingChoice) {
    int hashValue = 0;

    switch (hashingChoice) {
    case 1:
        hashValue = divisionMethod(key, tableSize);
        cout << "Hash value using Division Method: " << hashValue << endl;
        break;
    case 2:
        hashValue = multiplicationMethod(key, tableSize);
        cout << "Hash value using Multiplication Method: " << hashValue << endl;
        break;
    case 3:
        hashValue = midSquareMethod(key, tableSize);
        cout << "Hash value using Mid Square Method: " << hashValue << endl;
        break;
    case 4:
        hashValue = foldingMethod(key, tableSize);
        cout << "Hash value using Folding Method: " << hashValue << endl;
        break;
    default:
        cout << "Invalid choice!" << endl;
        break;
    }
}

// 2. Collision Resolution Techniques

// Updated Chaining Class
class HashTableChaining {
public:
    vector<list<int>> table;
    int tableSize;
    int elementCount; // Keeps track of the number of elements
    float loadFactorThreshold; // Load factor threshold for rehashing

    // Constructor to initialize the hash table with a user-defined load factor threshold
    HashTableChaining(int size, float threshold) {
        tableSize = size;
        loadFactorThreshold = threshold;
        elementCount = 1;
        table.resize(size);
        cout << "Hash Table created with size " << size << endl; // Debugging Output
    }

    // Function to check if rehashing is needed based on load factor threshold
    bool needsRehashing() {
        float loadFactor = float(elementCount) / tableSize;
        cout << "Load Factor: " << loadFactor << " (Threshold: " << loadFactorThreshold << ")" << endl; // Debugging output
        return loadFactor > loadFactorThreshold;
    }

    // Function to rehash the table when the load factor exceeds the threshold
    void rehash() {
        cout << "Rehashing triggered. Doubling table size." << endl; // Debugging output
        vector<list<int>> oldTable = table;
        tableSize *= 2; // Double the table size
        table.clear();
        table.resize(tableSize);
        elementCount = 0; // Reset element count

        // Reinsert all elements into the new table
        for (const auto& chain : oldTable) {
            for (int key : chain) {
                insert(key); // Insert each key into the new table
            }
        }
        cout << "Rehashing completed. New table size: " << tableSize << endl; // Debugging Output
    }

    // Function to insert a key into the hash table
    void insert(int key) {
        cout << "Attempting to insert key: " << key << endl; // Debugging Output
        // Check if rehashing is needed before inserting
        if (needsRehashing()) {
            rehash();  // Rehash if necessary
        }

        int index = divisionMethod(key, tableSize);

        // Prevent duplicate entries
        if (find(table[index].begin(), table[index].end(), key) == table[index].end()) {
            table[index].push_back(key);
            elementCount++;
            cout << "Key " << key << " inserted at index " << index << endl; // Debugging output
        } else {
            cout << "Key " << key << " already exists in the hash table." << endl; // Debugging output
        }
    }

    // Function to display the hash table
    void display() {
        for (int i = 0; i < tableSize; i++) {
            cout << i << ": ";
            for (int key : table[i]) {
                cout << key << " ";
            }
            cout << endl;
        }
    }
};




// Open Addressing Method
class HashTableOpenAddressing {
public:
    vector<int> table;
    int tableSize;
    int elementCount; // Keeps track of the number of elements
    int EMPTY = -1;
    int DELETED = -2;
    float loadFactorThreshold;

    HashTableOpenAddressing(int size, float threshold) {
        tableSize = size;
        loadFactorThreshold = threshold;
        table.resize(size, EMPTY);
        elementCount = 0;
        cout << "Hash Table created with size " << size << " and load factor threshold " << threshold << endl;
    }

    // Function to calculate the load factor
    float loadFactor() {
        return (float(elementCount) / tableSize);
    }

    // Function to check if rehashing is needed based on load factor
    bool needsRehashing() {
        return loadFactor() > loadFactorThreshold;
    }

    // Function to resize and rehash the table
    void rehash() {
        cout << "Rehashing triggered. Doubling table size." << endl;
        vector<int> oldTable = table;
        tableSize *= 2; // Double the table size
        table.clear();
        table.resize(tableSize, EMPTY);
        elementCount = 0; // Reset element count

        // Reinsert all elements into the new table using the current probing method
        for (int key : oldTable) {
            if (key != EMPTY && key != DELETED) {
                insert(key, false); // Reinsert using linear probing by default
            }
        }
        cout << "Rehashing completed. New table size: " << tableSize << endl;
    }

    // Linear Probing
    int linearProbing(int key) {
        int index = divisionMethod(key, tableSize);
        while (table[index] != EMPTY && table[index] != DELETED) {
            index = (index + 1) % tableSize;
        }
        return index;
    }

    // Quadratic Probing
    int quadraticProbing(int key) {
        int index = divisionMethod(key, tableSize);
        int i = 1;
        while (table[index] != EMPTY && table[index] != DELETED) {
            index = (divisionMethod(key, tableSize) + i * i) % tableSize;
            i++;
            if (i == tableSize) { // Prevent infinite loop if table is full
                return -1;
            }
        }
        return index;
    }

    // Division method for hashing
    int divisionMethod(int key, int size) {
        return key % size;
    }

    // Insert function with user-selected probing method
    void insert(int key, bool useQuadratic = false) {
        if (needsRehashing()) {
            rehash();  // Rehash if necessary
        }

        int index;
        if (useQuadratic) {
            index = quadraticProbing(key); // Quadratic probing
        } else {
            index = linearProbing(key); // Linear probing
        }

        if (index != -1) { // Ensure a valid index is found
            table[index] = key;
            elementCount++;
            cout << "Key " << key << " inserted at index " << index << endl;
        } else {
            cout << "Failed to insert key: " << key << ". Table is full!" << endl;
        }

        // After each insert, check if rehashing is needed
        if (needsRehashing()) {
            rehash();  // Trigger rehashing if necessary after insertion
        }
    }

    // Display the hash table
    void display() {
        cout << "Current Hash Table:" << endl;
        for (int i = 0; i < tableSize; i++) {
            if (table[i] != EMPTY) {
                cout << i << ": " << table[i] << endl;
            }
        }
    }
};


class HashTableDoubleHashing {
public:
    vector<int> table;
    int tableSize;
    int elementCount;
    int EMPTY = -1;
    int DELETED = -2;
    float loadFactorThreshold = 0.75; // Threshold for rehashing

    HashTableDoubleHashing(int size) {
        tableSize = size;
        table.resize(size, EMPTY);
        elementCount = 0;
    }

    // Function to check if rehashing is needed
    bool needsRehashing() {
        return (float(elementCount) / tableSize) > loadFactorThreshold;
    }

    // Function to resize and rehash the table
    void rehash() {
        vector<int> oldTable = table;
        tableSize *= 2; // Double the table size
        table.clear();
        table.resize(tableSize, EMPTY);
        elementCount = 0; // Reset element count

        // Reinsert all elements into the new table
        for (int key : oldTable) {
            if (key != EMPTY && key != DELETED) {
                insert(key);
            }
        }
        cout << "Rehashing completed. New table size: " << tableSize << endl;
    }

    int firstHash(int key) {
        return key % tableSize;
    }

    int secondHash(int key) {
        return 1 + (key % (tableSize - 1));  // Ensure step size is not zero
    }

    void insert(int key) {
        if (needsRehashing()) {
            rehash();
        }

        int index = firstHash(key);
        int stepSize = secondHash(key);
        while (table[index] != EMPTY && table[index] != DELETED) {
            index = (index + stepSize) % tableSize;
        }
        table[index] = key;
        elementCount++;
    }

    void display() {
        for (int i = 0; i < tableSize; i++) {
            if (table[i] != EMPTY) {
                cout << i << ": " << table[i] << endl;
            }
        }
    }
};


int main() {
    int tableSize = 11;  // Set a default table size
    int key;
    int probingChoice;

    // Test Division Method
    cout << "Testing Division Method:" << endl;
    cout << "Enter a key to hash: ";
    cin >> key;
    int divisionHash = divisionMethod(key, tableSize);
    cout << "Hash value using Division Method: " << divisionHash << endl;
    cout << endl;

    // Test Multiplication Method
    cout << "Testing Multiplication Method:" << endl;
    cout << "Enter a key to hash: ";
    cin >> key;
    int multiplicationHash = multiplicationMethod(key, tableSize);
    cout << "Hash value using Multiplication Method: " << multiplicationHash << endl;
    cout << endl;

    // Test Mid Square Method
    cout << "Testing Mid Square Method:" << endl;
    cout << "Enter a key to hash: ";
    cin >> key;
    int midSquareHash = midSquareMethod(key, tableSize);
    cout << "Hash value using Mid Square Method: " << midSquareHash << endl;
    cout << endl;

    // Test Folding Method
    cout << "Testing Folding Method:" << endl;
    cout << "Enter a key to hash: ";
    cin >> key;
    int foldingHash = foldingMethod(key, tableSize);
    cout << "Hash value using Folding Method: " << foldingHash << endl;
    cout << endl;

    // Test HashTableChaining
    float loadFactorThreshold = 2;
    cout << "Testing HashTableChaining:" << endl;
    HashTableChaining chainTable(tableSize, loadFactorThreshold);

    // Insert keys into the chained hash table
    chainTable.insert(12);
    chainTable.insert(10);
    chainTable.insert(7);
    chainTable.insert(14);
    chainTable.insert(5);
    chainTable.insert(11);
    chainTable.insert(22);
    // Display the hash table after insertions
    cout << "\nHash Table (Chaining) after insertions:" << endl;
    chainTable.display();
    cout << endl;

    // Test HashTableOpenAddressing with Linear or Quadratic Probing
    cout << "Testing HashTableOpenAddressing:" << endl;
    cout << "Choose probing method:" << endl;
    cout << "1. Linear Probing" << endl;
    cout << "2. Quadratic Probing" << endl;
    cout << "Enter your choice (1-2): ";
    cin >> probingChoice;

    HashTableOpenAddressing openTable(tableSize, loadFactorThreshold);

    // Insert keys using the chosen probing method
    openTable.insert(7, probingChoice == 2);  // Use quadratic probing if choice is 2
    openTable.insert(36, probingChoice == 2);
    openTable.insert(18, probingChoice == 2);
    openTable.insert(62, probingChoice == 2);
    openTable.insert(51, probingChoice == 2);

    // Display the hash table after insertions
    cout << "\nHash Table (Open Addressing) after insertions:" << endl;
    openTable.display();
    cout << endl;

    // Test HashTableDoubleHashing
    cout << "Testing HashTableDoubleHashing:" << endl;
    HashTableDoubleHashing doubleHashingTable(tableSize);

    // Insert keys into the double hashing table
    doubleHashingTable.insert(37);
    doubleHashingTable.insert(90);
    doubleHashingTable.insert(45);
    doubleHashingTable.insert(22);

    // Display the hash table after insertions
    cout << "\nHash Table (Double Hashing) after insertions:" << endl;
    doubleHashingTable.display();

    return 0;
}
