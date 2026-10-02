#include <iostream>
#include <random>

using namespace std;

template <typename T>
struct Node {
    int key;
    T value;
    int priority;
    Node<T>* left = nullptr;
    Node<T>* right = nullptr;
    Node<T>* parent = nullptr;

    inline static mt19937 gen{random_device{}()};

    Node(int k, T v, int p = -1) : key{k}, value{v} {
        priority = (p >= 0) ? p : uniform_int_distribution<int>(1, 10000)(gen);
    }

    bool has_left() {
        // if left is something other than null, that means there is a child
        return left != nullptr;
    }

    bool has_right() {
        return right != nullptr;
    }

    bool has_parent() {
        return parent != nullptr;
    }

    bool is_leaf() {
        // If it doesn't have a left child and it doesn't have a right child, it
        // must be a leaf node.
        return !has_left() && !has_right();
    }
};

template <typename T>
struct BinarySearchTree {
    Node<T>* root = nullptr;

    /// Return the node in the tree with this value. If there is no such node, return nullptr.
    Node<T>* search(int key) {
        return _search_helper(root, key);
    }

    Node<T>* _search_helper(Node<T>* current_node, int key) {
        // TODO
        if (current_node == nullptr) return current_node;

        if (current_node->key == key)
        {
            return current_node;
        }

        if (current_node->key > key)
        {
            return _search_helper(current_node->left, key);
        }

            return _search_helper(current_node->right, key);

    }

    void left_rotate(Node<T>* current_node)
    {
        Node<T>* right_child = current_node->right;

        current_node->right = right_child->left;
        if (right_child->left != nullptr) right_child->left->parent = current_node;

        right_child->left = current_node;

        right_child->parent = current_node->parent;

        if (current_node->parent == nullptr)
        {
            this->root = right_child;
            current_node->parent = right_child;

        }
        else if (current_node == current_node->parent->left)
        {
            current_node->parent->left = right_child;
            current_node -> parent = right_child;
        }
        else if (current_node == current_node->parent->right)
        {
            current_node->parent->right = right_child;
            current_node -> parent = right_child;
        }
    }

    void right_rotate(Node<T>* current_node)
    {
        Node<T>* left_child = current_node->left;

        current_node->left = left_child->right;

        if (left_child->right != nullptr) left_child->right->parent = current_node;

        left_child->right = current_node;

        left_child->parent = current_node->parent;

        if (current_node->parent == nullptr)
        {
            this->root = left_child;
            current_node->parent = left_child;
        }
        else if (current_node == current_node->parent->left)
        {
            current_node->parent->left = left_child;
            current_node -> parent = left_child;
        }
        else if (current_node == current_node->parent->right)
        {
            current_node->parent->right = left_child;
            current_node -> parent = left_child;
        }
    }

    void insert(int key, T value, int priority = -1) {
        Node<T>* val = new Node<T>(key, value, priority);
        // First, if the tree is empty, just make this the root node
        if (root == nullptr) {
            root = val;
            return;
        }
        // Again, I need a helper function to handle my recursion input
        return _insert_helper(root, val);
    }

    void _insert_helper(Node<T>* current_node, Node<T>* inserting) {
        // TODO
        if (current_node->key >= inserting->key)
        {
            if (current_node->left == nullptr)
            {
                current_node->left = inserting;
                inserting->parent = current_node;
            }else
            {
                _insert_helper(current_node->left, inserting);
            }

            if (current_node->left->priority > current_node->priority)
            {
                right_rotate(current_node);
            }

        }


        else if (current_node->key < inserting->key)
        {
            if (current_node->right == nullptr)
            {
                current_node->right = inserting;
                inserting->parent = current_node;
            }else
            {
                _insert_helper(current_node->right, inserting);
            }

            if (current_node->right->priority > current_node->priority)
            {
                left_rotate(current_node);
            }
        }
    }

    void remove(int key) {
        return _remove_helper(root, key);
    }

    void _remove_helper(Node<T>* current_node, int key) {
        // TODO
        if (current_node == nullptr)
        {
            return;
        }

        if (current_node->key > key)
        {
            _remove_helper(current_node->left, key);
        }
        else if (current_node->key < key)
        {
            _remove_helper(current_node->right, key);
        }
        else
        {
            current_node->priority = 0;

            while (! current_node->is_leaf())
            {
                if (current_node->has_left() && current_node->has_right())
                {
                    if (current_node->left->priority >= current_node->right->priority)
                    {
                        right_rotate(current_node);
                        continue;
                    }

                    if (current_node->right->priority > current_node->left->priority)
                    {
                        left_rotate(current_node);
                        continue;
                    }
                }

                else if (current_node->has_left()) right_rotate(current_node);

                else if (current_node->has_right()) left_rotate(current_node);
            }

            Node<T>* parent = current_node->parent;
            if (parent == nullptr)
            {
                root = nullptr;
                delete current_node;
            }

            else if (parent->left == current_node)
            {
                delete current_node;
                parent->left = nullptr;
            }

            else
            {
                delete current_node;
                parent->right = nullptr;
            }
        }
    }

    void update(int key, T value) {
        Node<T>* node = search(key);
        if (node) node->value = value;
        else insert(key, value);
    }
};

int ontrack_test() {
    // Ontrack tests, do not change
    BinarySearchTree<string> bst;
    Node<string>* context_node = nullptr;
    string menu = "0. Show this menu\n1. Insert a key\n2. Update a key\n3. Search for a key\n4. Remove a key\n5. Exit";
    int command = 0;
    while (command != 5) {
        if (command == 0) {
            cout << menu << endl;
        } else if (command == 1) {
            //insert
            int key;
            string value;
            int priority;
            cout << "Enter key, value, priority: " << endl;
            cin >> key;
            cin >> value;
            cin >> priority;
            bst.insert(key, value, priority);
        } else if (command == 2) {
            //update
            int key;
            string value;
            cout << "Enter key, value: " << endl;
            cin >> key;
            cin >> value;
            bst.update(key, value);
        } else if (command == 3) {
            //search
            cout << "Enter key to search: " << endl;
            int key;
            cin >> key;
            context_node = bst.search(key);
            if (context_node) {
                cout << "Found node (" << context_node->key << ", " << context_node->value << ", " << context_node->priority << ")\n";
                cout << "Parent: " << (context_node->parent ? to_string(context_node->parent->key) : "nullptr") << " Left: " << (context_node->left ? to_string(context_node->left->key) : "nullptr") << " Right: " << (context_node->right ? to_string(context_node->right->key) : "nullptr") << endl;
            } else cout << "Could not find node with key: " << key << endl;
        } else if (command == 4) {
            //remove
            cout << "Enter key to remove: " << endl;
            int key;
            cin >> key;
            bst.remove(key);
        }
        cout << "Enter command (0-5)" << endl;
        if (!(cin >> command)) {
            cout << "ERROR: could not read command" << endl;
            return 1;
        }
    }
    return 0;
}

/*
int main(int argc, char** argv) {
    if (argc == 2) return ontrack_test();

    BinarySearchTree<string> bst;
    bst.insert(3, "a", 8);
    bst.insert(11, "b", 6);
    bst.insert(8, "c", 10);
    // At this point, c should be the root, and a/b should be left/right
    cerr << "Root: (" << bst.root->value << ", " << bst.root->priority << ")" << endl;
    cerr << "Left: (" << bst.root->left->value << ", " << bst.root->left->priority << ")" << endl;
    cerr << "Right: (" << bst.root->right->value << ", " << bst.root->right->priority << ")" << endl;
    bst.insert(7, "d", 5);
    bst.insert(9, "e", 4);
    bst.insert(5, "f", 2);
    // Now we should see the final structure of the tree.
    cerr << "===" << endl;
    cerr << "Root: (" << bst.root->value << ", " << bst.root->priority << ")" << endl; // c
    cerr << "Left: (" << bst.root->left->value << ", " << bst.root->left->priority << ")" << endl; // a
    cerr << "Right: (" << bst.root->right->value << ", " << bst.root->right->priority << ")" << endl; // b
    cerr << "Left->Right: (" << bst.root->left->right->value << ", " << bst.root->left->right->priority << ")" << endl; // d
    cerr << "Left->Right->Left: (" << bst.root->left->right->left->value << ", " << bst.root->left->right->left->priority << ")" << endl; // f
    cerr << "Right->Left: (" << bst.root->right->left->value << ", " << bst.root->right->left->priority << ")" << endl; // e

    return 0;
}
*/


// Tests:

int main()
{
    BinarySearchTree<string> bst;

    bst.insert(1, "a", 4);
    bst.insert(5, "b", 9);
    bst.insert(3, "c", 6);
    bst.insert(2, "d", 8);
    bst.insert(4, "e", 11);

    cout << "Root: "
         << bst.root->key << " "
         << bst.root->value << " "
         << bst.root->priority << endl;

    cout << "Root left: "
         << bst.root->left->key << endl;

    cout << "Root right: "
         << bst.root->right->key << endl;

    cout << "Root left left: "
         << bst.root->left->left->key << endl;

    cout << "Root left right: "
         << bst.root->left->right->key << endl;

    cout << "\n--- Search Test ---" << endl;

    Node<string>* result1 = bst.search(3);

    if (result1 != nullptr)
    {
        cout << "Found key 3: " << result1->value << endl;
    }
    else
    {
        cout << "Key 3 not found" << endl;
    }

    Node<string>* result2 = bst.search(100);

    if (result2 != nullptr)
    {
        cout << "Found key 100: " << result2->value << endl;
    }
    else
    {
        cout << "Key 100 not found" << endl;
    }

    cout << "\n--- Remove Non-existing Key Test ---" << endl;

    bst.remove(100);

    cout << "Root after remove(100): " << bst.root->key << endl;
    cout << "Search 100: "
         << (bst.search(100) == nullptr ? "Not found" : "Found")
         << endl;


    cout << "\n--- Remove Leaf Test ---" << endl;

    bst.remove(1);

    cout << "Search 1: "
         << (bst.search(1) == nullptr ? "Not found" : "Found")
         << endl;

    cout << "Root: " << bst.root->key << endl;
    cout << "Root left: " << bst.root->left->key << endl;
    cout << "Root right: " << bst.root->right->key << endl;
    cout << "Root left right: " << bst.root->left->right->key << endl;

    cout << "\n--- Remove One-child Test ---" << endl;

    bst.remove(2);

    cout << "Search 2: "
         << (bst.search(2) == nullptr ? "Not found" : "Found")
         << endl;

    cout << "Search 3: "
         << (bst.search(3) == nullptr ? "Not found" : "Found")
         << endl;

    cout << "Root: " << bst.root->key << endl;
    cout << "Root left: " << bst.root->left->key << endl;
    cout << "Root right: " << bst.root->right->key << endl;

    cout << "Root left parent: "
         << bst.root->left->parent->key << endl;

    cout << "\n--- Remove Two-children Root Test ---" << endl;

    BinarySearchTree<string> bst2;

    bst2.insert(1, "a", 4);
    bst2.insert(5, "b", 9);
    bst2.insert(3, "c", 6);
    bst2.insert(2, "d", 8);
    bst2.insert(4, "e", 11);

    bst2.remove(4);

    cout << "Search 4: "
         << (bst2.search(4) == nullptr ? "Not found" : "Found")
         << endl;

    cout << "Root: " << bst2.root->key << endl;
    cout << "Root left: " << bst2.root->left->key << endl;
    cout << "Root left left: "
         << bst2.root->left->left->key << endl;
    cout << "Root left right: "
         << bst2.root->left->right->key << endl;

    cout << "Root parent is nullptr: "
         << (bst2.root->parent == nullptr ? "Yes" : "No")
         << endl;

    cout << "Root left parent: "
         << bst2.root->left->parent->key << endl;

    cout << "\n--- Remove Single Root Test ---" << endl;

    BinarySearchTree<string> bst3;
    bst3.insert(10, "only", 100);

    bst3.remove(10);

    cout << "Root is nullptr: "
         << (bst3.root == nullptr ? "Yes" : "No")
         << endl;

    cout << "Search 10: "
         << (bst3.search(10) == nullptr ? "Not found" : "Found")
         << endl;

    cout << "\n--- Remove All Test ---" << endl;

    BinarySearchTree<string> bst4;

    bst4.insert(1, "a", 4);
    bst4.insert(5, "b", 9);
    bst4.insert(3, "c", 6);
    bst4.insert(2, "d", 8);
    bst4.insert(4, "e", 11);

    bst4.remove(1);
    bst4.remove(2);
    bst4.remove(3);
    bst4.remove(4);
    bst4.remove(5);

    cout << "Tree is empty: "
         << (bst4.root == nullptr ? "Yes" : "No")
         << endl;

    cout << "Search 1: "
         << (bst4.search(1) == nullptr ? "Not found" : "Found")
         << endl;

    cout << "Search 5: "
         << (bst4.search(5) == nullptr ? "Not found" : "Found")
         << endl;

    cout << "\n--- Remove Single-left-child Test ---" << endl;

    BinarySearchTree<string> bst5;

    bst5.insert(10, "a", 10);
    bst5.insert(5, "b", 5);

    bst5.remove(10);

    cout << "Search 10: "
         << (bst5.search(10) == nullptr ? "Not found" : "Found")
         << endl;

    cout << "Search 5: "
         << (bst5.search(5) == nullptr ? "Not found" : "Found")
         << endl;

    cout << "Root: " << bst5.root->key << endl;

    cout << "Root parent is nullptr: "
         << (bst5.root->parent == nullptr ? "Yes" : "No")
         << endl;

    return 0;
}


