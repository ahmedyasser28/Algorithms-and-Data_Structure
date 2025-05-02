#include <bits/stdc++.h>
using namespace std;

int nthLine = 0;
fstream IndexFile;
vector<string> BtreeVector;


class Node{
public:
    vector<pair<int , int >> Records;
    bool leaf;
    int splitCount = 0;
    int line = -1;
    vector<Node *> children;

    Node(bool l , int max) : children(max , nullptr){
        this->leaf = l;
        for(int i = 0; i < max; i++)
            children[i] = nullptr;
    }

    int InsertRecordToNode(int id, int reference) {

        int i = 0;
        while (i < Records.size() && id > Records[i].first)
            i++;

        Records.insert(Records.begin() + i, std::make_pair(id, reference));

        return i;
    }

    bool operator<(const Node &other) const {
        return Records < other.Records;
    }
};


void InitializeBtreeFile(int NumOfNodes, int max) {
    BtreeVector.resize(NumOfNodes); // Resize the vector
    for (int i = 0; i < NumOfNodes; i++) {
        string line = to_string(-1) + '|';
        if (i == NumOfNodes - 1) {
            line += to_string(-1) + '|';
        } else {
            line += to_string(1 + i) + '|';
        }
        for (int j = 0; j < (max * 2) - 1; j++) {
            line += to_string(-1) + '|';
        }
        BtreeVector[i] = line;
    }
}

void CreateIndexFile(const char * Fname , int NumOfNodes , int max){
    IndexFile.open(Fname , ios::binary | ios::out);
    for (int i = 0; i < NumOfNodes; ++i) {
        IndexFile << BtreeVector[i] << "\n";
    }
    IndexFile.close();
}

string writeRecord(Node *node, int max) {
    string record = "";
    int count = 0;
    record += to_string(node->leaf) + '|';

    for (const auto &Rec: node->Records) {
        if (Rec != make_pair(-1, -1)) {
            record += to_string(Rec.first) + '|' + to_string(Rec.second) + '|';
            count += 2;
        }
    }

    while (count < max * 2) {
        record += to_string(-1) + '|';
        count++;
    }
    return record;
}

void Sort(vector<Node *> &Nodes , int n){
    int MinIndex;

    for (int i = 0; i < n - 1; ++i) {
        MinIndex = i;
        for (int j = i + 1; j < n; ++j) {
            if (Nodes[j]->splitCount < Nodes[MinIndex]->splitCount)
                MinIndex = j;
        }
        swap(Nodes[MinIndex] , Nodes[i]);
    }
}

void writeFile(string Fname , int max , Node *root , int NumOfNodes){
    IndexFile.open(Fname , ios::out);
    string Record;
    vector<Node *> splitedNodes;
    if(root == nullptr)
        return;

    queue<Node *> Nodes;
    Nodes.push(root);
    while(!Nodes.empty()){
        int n = Nodes.size();
        for (int i = 0; i < n; ++i) {
            Node *current = Nodes.front();
            Nodes.pop();

            if(current->splitCount > 0)
                splitedNodes.push_back(current);
            else {
                nthLine++;
                Record = writeRecord(current , max);
                current->line = nthLine;
                if (current->line >= 0 && current->line < BtreeVector.size()) {
                    BtreeVector[current->line] = Record;
                }
                else {
                    cout << "Error: Line number out of range\n";
                    return;
                }
            }
            for (int j = 0; j < current->children.size(); ++j) {
                if (current->children[j] != nullptr){
                    Nodes.push(current->children[j]);
                }
            }
        }
    }
    Sort(splitedNodes , splitedNodes.size());
    for(auto &current : splitedNodes){
        Record = writeRecord(current , max);
        nthLine++;
        current->line = nthLine;
        if(current != nullptr && current->line != -1 && current->line < BtreeVector.size()) BtreeVector[current->line] = Record;
    }
    nthLine = 0;
    IndexFile.close();
}
void offset_setter(int maxKeys, Node *root) {
    int c = 0;
    if (root->leaf == 1) {
        for (auto &i: root->Records) {
            i.second = root->children[c]->line;
            c++;
        }
    }
    for (Node *child: root->children) {
        if (child != nullptr) {
            offset_setter(maxKeys, child);
        }
    }
}

void emptyNodeIndex(string filename, int maxKeys) {

    IndexFile.open(filename, ios::out | ios::in);
    bool exist = false;
    int cnt = 0;
    for (string rec: BtreeVector) {
        if (rec[0] == '-' && cnt != 0) {
            exist = true;
            break;
        }
        cnt++;
    }

    if (!exist) {
        cnt = -1;
    }
    string s = "-1|" + to_string(cnt) + '|';
    for (int i = 0; i < maxKeys * 2 - 1; i++) {
        s += "-1|";
    }
    s += " ";
    BtreeVector[0] = s;

    IndexFile.close();
}

void fileUpdate(const char *filename, Node *root, int numberOfRecords, int maxKeys) {
    InitializeBtreeFile(numberOfRecords,maxKeys);
    writeFile(filename, maxKeys, root,numberOfRecords);
    offset_setter(maxKeys, root);
    writeFile(filename, maxKeys, root,numberOfRecords);
    emptyNodeIndex(filename, maxKeys);
    CreateIndexFile(filename, 10, maxKeys);
    IndexFile.flush();
}

class BTree {
public:
    Node *root;
    int max_keys;
    int numberOfRecords;


    BTree(int m, int numofrecods) {
        this->max_keys = m;
        root = nullptr;
        this->numberOfRecords = numofrecods;

    }
    int InsertNewRecordAtIndex(const char *filename, int RecordID, int Reference) {

        if (root == nullptr) {
            root = new Node(0, max_keys);
            root->InsertRecordToNode(RecordID, Reference);
            fileUpdate(filename, root, numberOfRecords, max_keys);
            return 1;
        }
        Node *node = root;
        Node *parent = nullptr;

        while (node->leaf == 1) {
            parent = node;

            int i = 0;
            while (i < node->Records.size() - 1 && RecordID > node->Records[i].first) {
                i++;
            }

            if (i < node->children.size() && node->children[i] != nullptr) {
                node = node->children[i];
            } else {
                break; // No more child nodes to traverse
            }
        }

        node->InsertRecordToNode(RecordID, Reference);

        if (parent != nullptr) {
            // Update parent node with the maximum value from the child
            int maxKeyIndex = parent->Records.size() - 1;
            if (parent->Records[maxKeyIndex].first < node->Records[node->Records.size() - 1].first) {
                parent->Records[maxKeyIndex] = node->Records[node->Records.size() - 1];
            }
        }

        if (node->Records.size() > max_keys) {
            if (!split(node)) {
                return -1;  // Indicate failure due to lack of space.
            }
        }


        fileUpdate(filename, root, numberOfRecords, max_keys);
        return node->line;
    }

    bool split(Node *node) {
        if (!node) return false;

        Node *left = new (nothrow) Node(node->leaf, max_keys);
        Node *right = new (nothrow) Node(node->leaf, max_keys);

        if (!left || !right) {
            delete left;
            delete right;
            return false;  // Memory allocation failed, return failure.
        }

        int mid = node->Records.size() / 2;
        for (int i = 0; i < mid; i++) {
            left->InsertRecordToNode(node->Records[i].first, node->Records[i].second);
        }
        for (int i = mid; i < node->Records.size(); i++) {
            right->InsertRecordToNode(node->Records[i].first, node->Records[i].second);
        }

        if (node->leaf == 1) {
            for (int i = 0; i < mid; i++) {
                left->children[i] = node->children[i];
            }
            for (int i = mid; i < node->Records.size(); i++) {
                right->children[i - mid] = node->children[i];
            }
        }

        if (node == root) {
            root = new (nothrow) Node(1, max_keys);
            if (!root) {
                delete left;
                delete right;
                return false;
            }

            left->splitCount++;
            right->splitCount++;
            root->InsertRecordToNode(node->Records[mid - 1].first, node->Records[mid - 1].second);
            root->InsertRecordToNode(node->Records[node->Records.size() - 1].first, node->Records[node->Records.size() - 1].second);
            root->children[0] = left;
            root->children[1] = right;

        } else {
            Node *parent = getParent(root, node);
            if (!parent) {
                delete left;
                delete right;
                return false;
            }

            int i = 0;
            while (i < parent->Records.size() - 1 && node->Records[mid].first > parent->Records[i].first) {
                i++;
            }

            parent->InsertRecordToNode(node->Records[mid - 1].first, node->Records[mid - 1].second);

            for (int j = parent->Records.size() - 1; j > i; j--) {
                parent->children[j + 1] = parent->children[j];
            }

            parent->children[i] = left;
            parent->children[i + 1] = right;

            if (parent->Records.size() > max_keys) {
                return split(parent);
            }
        }

        return true;
    }

    Node *getParent(Node *node, Node *child) {
        if (node == nullptr || node->leaf == false) {
            return nullptr;
        }
        for (int i = 0; i < max_keys; i++) {
            if (node->children[i] == child) {
                return node;
            }
            Node *parent = getParent(node->children[i], child);
            if (parent != nullptr) {
                return parent;
            }
        }
        return NULL;
    }

    //search function
    int SearchARecord(const char* filename, int key) {
        if (root == nullptr) {
            return -1;
        }

        Node *node = root;

        while (node->leaf == 1) {
            int i = 0;
            while (i < node->Records.size() && key > node->Records[i].first) {
                i++;
            }
            if (i < node->children.size() && node->children[i] != nullptr) {
                node = node->children[i];
            } else {
                break;
            }
        }
        int i = 0;
        while (i < node->Records.size() && key > node->Records[i].first) {
            i++;
        }
        if (i < node->Records.size() && node->Records[i].first == key) {
            return node->Records[i].second;
        }
        return -1;
    }

    Node *getNode(int key) {
        if (root == nullptr) {
            return nullptr;
        }

        Node *temp = root;
        int count = 0;
        while (count < numberOfRecords&&temp!= nullptr) {
            for (auto i: temp->Records) {
                if (i.first == key) {
                    return temp;
                }
            }
            int counter = 0;
            for (auto i: temp->Records) {
                if (i.first > key) {
                    temp = temp->children[counter];
                    break;
                }
                counter++;
            }
            count++;

        }
        return nullptr;
    }
    Node *getleafparent(int key) {
        if (root == nullptr) {
            return nullptr;
        }
        Node *pre=root;
        Node *temp = root;
        int count = 0;
        while (count < numberOfRecords) {
            for (auto i: temp->Records) {
                if (i.first == key) {
                    return pre;
                }
            }
            int counter = 0;
            for (auto i: temp->Records) {
                if (i.first > key) {
                    pre=temp;
                    temp = temp->children[counter];
                    break;
                }
                counter++;
            }
            count++;

        }
        return nullptr;
    }

    void DeleteRecordFromIndex(const char *filename, int RecordID) {
        Node *target = getNode(RecordID);
        if (target == nullptr) {
            cout << "key not found \n";
            return;
        }
        if (target->leaf == 0) {
            if (target->Records.size() > 2) {
                for (int i = 0; i < target->Records.size(); i++) {
                    if (RecordID == target->Records[i].first) {
                        target->Records.erase(target->Records.begin() + i);
                        target->children.erase(target->children.begin() + i);
                    }
                }
            } else {
                for (int i = 0; i < target->Records.size(); i++) {
                    if (RecordID == target->Records[i].first) {
                        Node *parent = getleafparent(RecordID);
                        int j = 0,z=0,fin=0;
                        while (parent->children[j] != NULL) {
                            if(parent->Records[j].first<RecordID){
                                z++;
                            }
                            j++;
                        }
                        Node *sibling = parent->children[z-1];
                        fin=z-1;
                        if(z==0){
                            sibling=parent->children[z+1];
                            fin=z+1;
                        }
                        if (sibling->Records.size() <= 2) {
                            target->Records.erase(target->Records.begin() + i);
                            target->children.erase(target->children.begin() + i);
                            merge(target, sibling, parent);
                        } else {
                            if(z>0) {
                                int temp = parent->Records[fin].first;
                                int temp2 = parent->Records[fin].second;
                                target->Records.erase(target->Records.begin() + i);
                                target->children.erase(target->children.begin() + i);
                                DeleteRecordFromIndex(filename, temp);
                                InsertNewRecordAtIndex(filename, temp, temp2);
                            }
                            else if (z==0){
                                target->Records.erase(target->Records.begin() + i);
                                target->children.erase(target->children.begin() + i);
                                int tmp=target->Records[1].first;
                                int tmp2=target->Records[1].second;
                                target->Records.insert(target->Records.begin(),sibling->Records[0]);
                                sibling->Records.erase(sibling->Records.begin());
                                deletenleaf(*target,target->Records[1].first);
                                InsertNewRecordAtIndex(filename, tmp,tmp2);
                            }
                        }
                    }
                }
            }
        } else {
            Node *leaf = getLeaf(RecordID, target);
            if (leaf->Records.size() >= 3) {
                deletenleaf(*target, RecordID);
            }
            if (leaf->Records.size() == 2) {
                for (int i = 0; i < target->Records.size(); i++) {
                    if (RecordID == target->Records[i].first) {
                        Node *parent = getParent(RecordID, target);
                        int j = 0,z=0,fin;
                        while (parent->children[j] != NULL) {
                            if(parent->Records[j].first<RecordID){
                                z++;
                            }
                            j++;
                        }
                        Node *sibling = parent->children[z-1];
                        fin=z-1;
                        if(z==0){
                            sibling=parent->children[z+1];
                            fin=z+1;
                        }
                        if (sibling->Records.size() == 2) {
                            deletenleaf(*target, RecordID);
                            merge(leaf, sibling, parent);
                        } else {
                            if(z>0) {
                                int temp = parent->Records[fin].first;
                                int temp2 = parent->Records[fin].second;
                                deletenleaf(*target, RecordID);
                                DeleteRecordFromIndex(filename, temp);
                                InsertNewRecordAtIndex(filename, temp, temp2);
                            }
                            else if(z==0){
                                deletenleaf(*target, RecordID);
                                leaf->Records.insert(leaf->Records.begin(),sibling->Records[0]);
                                sibling->Records.erase(sibling->Records.begin());
                                int tmp=leaf->Records[1].first;
                                int tmp2=leaf->Records[1].second;
                                deletenleaf(*target,leaf->Records[1].first);
                                InsertNewRecordAtIndex(filename, tmp,tmp2);
                            }
                        }
                    }
                }
            }
        }
        fileUpdate(filename, root, numberOfRecords, max_keys);
    }
    void merge(Node *current, Node *sibling, Node *parent) {
        sibling->InsertRecordToNode(current->Records[0].first,current->Records[0].second);
        pair<int, int> x = current->Records[0];
        current->Records.pop_back();
        current->children.pop_back();
        current->leaf=-1;
        auto it =find_if(parent->Records.begin(), parent->Records.end(),
                         [&x](const pair<int, int>& element) { return element.first == x.first; });
        int index =distance(parent->Records.begin(), it);
        if (it != parent->Records.end()) {
            parent->Records.erase(parent->Records.begin()+index);
        }
        parent->children.erase(parent->children.begin()+index);
        parent->Records[index-1] = x;

    }

    Node *getParent(int key, Node *start) {
        Node *parent = nullptr;
        Node *temp = start;
        while (temp->leaf == 1) {
            int counter = 0;
            for (auto i: temp->Records) {
                if (i.first == key) {
                    parent = temp;
                    temp = temp->children[counter];
                }
                counter++;
            }
        }
        return parent;
    }

    Node *getLeaf(int key, Node *start) {
        Node *temp = start;
        while (temp->leaf == 1) {
            int counter = 0;
            for (auto i: temp->Records) {
                if (i.first == key) {
                    temp = temp->children[counter];
                }
                counter++;
            }
        }
        return temp;
    }


    pair<int, int> deletenleaf(Node &x, int key) {
        if (x.leaf == 0) {
            x.Records.pop_back();
            x.children.pop_back();
            //update index file
            return {x.Records[x.Records.size() - 1].first, x.Records[x.Records.size() - 1].second};
        }
        if (x.leaf == 1) {
            int counter = 0;
            for (auto &i: x.Records) {
                if (i.first == key) {
                    pair<int, int> replace = deletenleaf(*x.children[counter], key);
                    i.first = replace.first;
                    i.second = replace.second;
                    return replace;
                }
                counter++;
            }
        }
        return make_pair(-1, -1);
    }
};

void displayIndexFileContent(const char *filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Unable to open file " << filename << endl;
        return;
    }

    string line;
    int lineNumber = 1;

    cout << "Index File Content:" << endl;
    cout << "-------------------" << endl;

    while (getline(file, line)) {
        cout << "Line " << lineNumber << ": " << line << endl;
        lineNumber++;
    }

    file.close();
}


int main() {
    int maxKeys = 5;
    int numberOfRecords = 10;
    const char *filename = "index.dat";
    BTree b(5, 10);
    int choice = -1;
    InitializeBtreeFile(10, maxKeys); //fill vector
    CreateIndexFile(filename, numberOfRecords, maxKeys);
    b.InsertNewRecordAtIndex(filename, 3, 12);
    b.InsertNewRecordAtIndex(filename, 7, 24);
    b.InsertNewRecordAtIndex(filename, 10, 48);
    b.InsertNewRecordAtIndex(filename, 24, 60);
    b.InsertNewRecordAtIndex(filename, 14, 72);
    b.InsertNewRecordAtIndex(filename, 19, 84);
    b.InsertNewRecordAtIndex(filename, 30, 96);
    b.InsertNewRecordAtIndex(filename, 15, 108);
    b.InsertNewRecordAtIndex(filename, 1, 120);
    b.InsertNewRecordAtIndex(filename, 5, 132);

    do {
        cout << "1- Insert new record at index" << endl;
        cout << "2- Delete record from index " << endl;
        cout << "3- Display index file content " << endl;
        cout << "4- Search for a record" << endl;
        cout << "0- Exit \n" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        int key, reference;
        switch (choice) {
            case 1:
                cout << "Enter the record: \n";
                cin >> key;
                cout << "Enter the reference: \n";
                cin >> reference;
                b.InsertNewRecordAtIndex(filename,key, reference);
                cout << "Record is Insert Successfully...\n" << endl;
                break;

            case 2:
                cout << "Enter the key you want to delete: \n";
                cin >> key;
                b.DeleteRecordFromIndex(filename, key);
                break;

            case 3:
                displayIndexFileContent(filename);
                cout << endl;
                break;

            case 4:
                cout << "Enter the record you are looking for: \n";
                cin >> key;

                int result;
                result = b.SearchARecord(filename, key);

                if (result == -1)
                    cout << "Record Does Not Exist!\n\n";
                else {
                    cout << "Record is Found\n";
                    cout << "Reference of record is: " << result << "\n\n";
                }
                break;

            case 0:
                return 0;
                break;

            default:
                cout << "Wrong Input, Try Again\n\n";
                break;

        }
    } while (choice != 0);
    return 0;

}