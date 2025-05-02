#include <iostream>
using namespace std;

enum Color { RED, BLACK };

class Node {
public:
    int data;
    Color color;
    Node* left;
    Node* right;
    Node* parent;

    Node(int val) {
        data = val;
        color = RED;
        left = nullptr;
        right = nullptr;
        parent = nullptr;
    }
};

 class RedBlackTree {
  private:

    Node* root;

    void rotateLeft(Node*& node)
    {
        Node* child = node->right;
        node->right = child->left;
        if (node->right != nullptr)
            node->right->parent = node;
        child->parent = node->parent;
        if (node->parent == nullptr)
            root = child;
        else if (node == node->parent->left)
            node->parent->left = child;
        else
            node->parent->right = child;
        child->left = node;
        node->parent = child;
    }

    // Utility function: Right Rotation
    void rotateRight(Node*& node)
    {
        Node* child = node->left;
        node->left = child->right;
        if (node->left != nullptr)
            node->left->parent = node;
        child->parent = node->parent;
        if (node->parent == nullptr)
            root = child;
        else if (node == node->parent->left)
            node->parent->left = child;
        else
            node->parent->right = child;
        child->right = node;
        node->parent = child;
    }

    // Utility function: Fixing Insertion Violation
    void fixInsert(Node*& node)
    {
        Node* parent = nullptr;
        Node* grandparent = nullptr;
        while (node != root && node->color == RED
               && node->parent->color == RED) {
            parent = node->parent;
            grandparent = parent->parent;
            if (parent == grandparent->left) {
                Node* uncle = grandparent->right;
                if (uncle != nullptr
                    && uncle->color == RED) {
                    grandparent->color = RED;
                    parent->color = BLACK;
                    uncle->color = BLACK;
                    node = grandparent;
                }
                else {
                    if (node == parent->right) {
                        rotateLeft(parent);
                        node = parent;
                        parent = node->parent;
                    }
                    rotateRight(grandparent);
                    swap(parent->color, grandparent->color);
                    node = parent;
                }
            }
            else {
                Node* uncle = grandparent->left;
                if (uncle != nullptr
                    && uncle->color == RED) {
                    grandparent->color = RED;
                    parent->color = BLACK;
                    uncle->color = BLACK;
                    node = grandparent;
                }
                else {
                    if (node == parent->left) {
                        rotateRight(parent);
                        node = parent;
                        parent = node->parent;
                    }
                    rotateLeft(grandparent);
                    swap(parent->color, grandparent->color);
                    node = parent;
                }
            }
        }
        root->color = BLACK;
    }

    // Utility function: Fixing Deletion Violation
 void fixDelete(Node*& node)
{
    while (node != root && (node == nullptr || node->color == BLACK)) {
        if (node == node->parent->left) {
            Node* sibling = node->parent->right;
            if (sibling != nullptr && sibling->color == RED) {
                sibling->color = BLACK;
                node->parent->color = RED;
                rotateLeft(node->parent);
                sibling = node->parent->right;
            }
            if ((sibling == nullptr || sibling->left == nullptr || sibling->left->color == BLACK) &&
                (sibling == nullptr || sibling->right == nullptr || sibling->right->color == BLACK)) {
                if (sibling != nullptr)
                    sibling->color = RED;
                node = node->parent;
            } else {
                if (sibling != nullptr && (sibling->right == nullptr || sibling->right->color == BLACK)) {
                    if (sibling->left != nullptr)
                        sibling->left->color = BLACK;
                    sibling->color = RED;
                    rotateRight(sibling);
                    sibling = node->parent->right;
                }
                if (sibling != nullptr) {
                    sibling->color = node->parent->color;
                    node->parent->color = BLACK;
                    if (sibling->right != nullptr)
                        sibling->right->color = BLACK;
                }
                rotateLeft(node->parent);
                node = root;
            }
        } else {
            // Symmetric logic for the right side
            Node* sibling = node->parent->left;
            if (sibling != nullptr && sibling->color == RED) {
                sibling->color = BLACK;
                node->parent->color = RED;
                rotateRight(node->parent);
                sibling = node->parent->left;
            }
            if ((sibling == nullptr || sibling->left == nullptr || sibling->left->color == BLACK) &&
                (sibling == nullptr || sibling->right == nullptr || sibling->right->color == BLACK)) {
                if (sibling != nullptr)
                    sibling->color = RED;
                node = node->parent;
            } else {
                if (sibling != nullptr && (sibling->left == nullptr || sibling->left->color == BLACK)) {
                    if (sibling->right != nullptr)
                        sibling->right->color = BLACK;
                    sibling->color = RED;
                    rotateLeft(sibling);
                    sibling = node->parent->left;
                }
                if (sibling != nullptr) {
                    sibling->color = node->parent->color;
                    node->parent->color = BLACK;
                    if (sibling->left != nullptr)
                        sibling->left->color = BLACK;
                }
                rotateRight(node->parent);
                node = root;
            }
        }
    }
    if (node != nullptr)
        node->color = BLACK;
}

    // Utility function: Find Node with Minimum Value
    Node* minValueNode(Node*& node)
    {
        Node* current = node;
        while (current->left != nullptr)
            current = current->left;
        return current;
    }
     Node* maxValueNode(Node*& node)
    {
        Node* current = node;
        while (current->right != nullptr)
            current = current->right;
        return current;
    }

    // Utility function: Transplant nodes in Red-Black Tree
    void transplant(Node*& root, Node*& u, Node*& v)
    {
        if (u->parent == nullptr)
            root = v;
        else if (u == u->parent->left)
            u->parent->left = v;
        else
            u->parent->right = v;
        if (v != nullptr)
            v->parent = u->parent;
    }

    // Utility function: Helper to print Red-Black Tree
    void printHelper(Node* root, string indent, bool last)
    {
        if (root != nullptr) {
            cout << indent;
            if (last) {
                cout << "R----";
                indent += "   ";
            }
            else {
                cout << "L----";
                indent += "|  ";
            }
            string sColor
                = (root->color == RED) ? "RED" : "BLACK";
            cout << root->data << "(" << sColor << ")"
                 << endl;
            printHelper(root->left, indent, false);
            printHelper(root->right, indent, true);
        }
    }

    // Utility function: Delete all nodes in the Red-Black
    // Tree
    void deleteTree(Node* node)
    {
        if (node != nullptr) {
            deleteTree(node->left);
            deleteTree(node->right);
            delete node;
        }
    }
     void inorder(Node *x) {
        if (x == nullptr)
            return;
        inorder(x->left);
        cout << x->data << " ";
        inorder(x->right);
    }

 public:

    RedBlackTree(){
         root=nullptr;
    }

    // Destructor: Delete Red-Black Tree
    ~RedBlackTree() { deleteTree(root); }


     void printInOrder() {
        cout << "Inorder: " << endl;
        if (root == nullptr)
            cout << "Tree is empty" << endl;
        else
            inorder(root);
        cout << endl;
    }
     int getminvalue() {
        if (root == nullptr) {
            cout << "Tree is empty" << endl;
            return -1; // Return a default value if the tree is empty
        }
        Node* minNode = minValueNode(root);
        return minNode->data;
    }

     int getmaxValue()
    {
        if (root == nullptr) {
            cout << "Tree is empty" << endl;
            return -1; // Return a default value if the tree is empty
        }
        Node* maxNode = maxValueNode(root);
        return maxNode->data;
    }

    void insert(int key)
    {
        Node* node = new Node(key);
        Node* parent = nullptr;
        Node* current = root;
        while (current != nullptr) {
            parent = current;
            if (node->data < current->data)
                current = current->left;
            else
                current = current->right;
        }
        node->parent = parent;
        if (parent == nullptr)
            root = node;
        else if (node->data < parent->data)
            parent->left = node;
        else
            parent->right = node;
        fixInsert(node);
    }

    // Public function: Remove a value from Red-Black Tree
     void remove(int key)
    {
        Node* node = root;
        Node* z = nullptr;
        Node* x = nullptr;
        Node* y = nullptr;
        while (node != nullptr) {
            if (node->data == key) {
                z = node;
                break; // Exit loop when the node is found
            }
            if (node->data <= key) {
                node = node->right;
            } else {
                node = node->left;
            }
        }

        if (z == nullptr) {
            cout << "Key "<<key<<" is not found in the tree" << endl;
            return;
        }

        y = z;
        Color yOriginalColor = y->color;
        if (z->left == nullptr) {
            x = z->right;
            transplant(root, z, z->right);
        } else if (z->right == nullptr) {
            x = z->left;
            transplant(root, z, z->left);
        } else {
            y = minValueNode(z->right);
            yOriginalColor = y->color;
            x = y->right;
            if (y->parent == z) {
                if (x != nullptr)
                    x->parent = y;
            } else {
                transplant(root, y, y->right);
                y->right = z->right;
                if (y->right != nullptr)
                    y->right->parent = y;
            }
            transplant(root, z, y);
            y->left = z->left;
            if (y->left != nullptr)
                y->left->parent = y;
            y->color = z->color;
        }
        delete z;
        if (yOriginalColor == BLACK && x != nullptr) {
            fixDelete(x);
        }
    }


    // Public function: Print the Red-Black Tree
    void printTree()
    {
        if (root == nullptr)
            cout << "Tree is empty." << endl;
        else {
            cout << "Red-Black Tree:" << endl;
            printHelper(root, "", true);
        }
    }
};


int main()
{
    RedBlackTree tree;


    tree.insert(7);
    tree.insert(3);
    tree.insert(18);
    tree.insert(10);
    tree.insert(22);
    tree.insert(8);
    tree.insert(11);
    tree.insert(26);
    tree.printTree();

     tree.remove(3);
    tree.remove(15);
    cout<<"tree after deletion"<<endl;
    tree.printTree();
    tree.printInOrder();



    return 0;
}