#include <iostream>
#include <climits>

using namespace std;

struct RangeAnswer {
    int min_value;
    int max_value;
};

// Enable debugging the RangeAnswer object.
// This way, you can do cerr << rangeAnswer and it will print the min/max values rather than erroring out.
ostream & operator << (ostream & outs, const RangeAnswer & vs) {
    return outs << "[" << vs.min_value << ", " << vs.max_value << "]";
}

struct Node {
    int value;
    Node* left = nullptr;
    Node* right = nullptr;

    bool has_left() {
        // if left is something other than null, that means there is a child
        return left != nullptr;
    }

    bool has_right() {
        return right != nullptr;
    }

    bool is_leaf() {
        // If it doesn't have a left child and it doesn't have a right child, it
        // must be a leaf node.
        return !has_left() && !has_right();
    }

    int node_count() {
        // TASK 1
        int count = 1;

        if (has_left())
        {
            count += left->node_count();
        }

        if (has_right())
        {
            count += right->node_count();
        }

        return count;

        /*
         * int left_count = (left == nullptr) ? 0 : left->node_count()
         * int right_count = (right == nullptr) ? 0 : right->node_count()
         *
         * return 1 + left_count + right_count
         */


    }

    RangeAnswer value_range() {
        // TASK 1
        RangeAnswer result = {this->value, this->value};

        if (is_leaf())
        {
            return result;
        }

        // only left child
        if (has_left() && !has_right())
        {
            result = this->left->value_range();
            if (this->value < result.min_value)
            {
                result.min_value = this->value;
            }
            if (this->value > result.max_value)
            {
                result.max_value = this->value;
            }
        }

        // only right child
        if (has_right() && !has_left())
        {
            result = this->right->value_range();
            if (this->value < result.min_value)
            {
                result.min_value = this->value;
            }
            if (this->value > result.max_value)
            {
                result.max_value = this->value;
            }
        }

        // both has right child and right child
        if (has_left() && has_right())
        {
            RangeAnswer left_result = this->left->value_range();
            RangeAnswer right_result = this->right->value_range();

            result = {
                (left_result.min_value < right_result.min_value) ? left_result.min_value : right_result.min_value ,
                (left_result.max_value > right_result.max_value) ? left_result.max_value : right_result.max_value
            };

            if (this->value < result.min_value)
            {
                result.min_value = this->value;
            }
            if (this->value > result.max_value)
            {
                result.max_value = this->value;
            }
        }

        return result;


    }

    // TASK 2 - Maintain these values correctly through insertion
    RangeAnswer stored_value_range = {INT_MAX, INT_MIN};
    int stored_node_count = 0;

};

struct BinarySearchTree {
    Node* root = nullptr;

    /// Return the node in the tree with this value. If there is no such node, return nullptr.
    Node* search(int value) {
        return _search_helper(root, value);
    }

    Node* _search_helper(Node* current_node, int value) {
        // If we are outside the tree, that means the value can't be found
        if (current_node == nullptr) return nullptr;
        // If current_node node has the value, return it
        if (current_node->value == value) return current_node;

        // Otherwise, check which child we should go to.
        if (value > current_node->value) {
            return _search_helper(current_node->right, value);
        } else {
            return _search_helper(current_node->left, value);
        }
    }

    bool insert(int value) {
        // First, if the tree is empty, just make this the root node
        if (root == nullptr) {
            root = new Node;
            root->value = value;

            root->stored_node_count = 1;
            root->stored_value_range = {value,value};

            // The item wasn't found, so we return true (indicating an insert happened)
            return true;
        }
        // Again, I need a helper function to handle my recursion input
        return _insert_helper(root, value);
    }

    bool _insert_helper(Node* current_node, int value) {
        // If this node has the key, return as the key is already in the tree.
        // Tell the caller that the key is not a new node.
        // We are defining that if the function returns false, that means the item was already in the tree
        if (current_node->value == value) return false;

        // Check which direction we should go
        if (value > current_node->value) {
            // We should go to the right
            // If there is no right child, that means this is exactly where
            // the new node should be inserted
            if (current_node->right == nullptr) {
                current_node->right = new Node;
                current_node->right->value = value;

                current_node->right->stored_node_count = 1;
                current_node->right->stored_value_range = {value,value};

                current_node->stored_node_count += 1;

                if (value < current_node->stored_value_range.min_value)
                {
                    current_node->stored_value_range.min_value = value;
                }

                if (value > current_node->stored_value_range.max_value)
                {
                    current_node->stored_value_range.max_value = value;
                }

                // Item was unique and an insertion happened, so we return true.
                return true;
            }
            // Otherwise, if there is a right child, continue there
            //return _insert_helper(current_node->right, value);
            bool inserted = _insert_helper(current_node->right, value);
            if (inserted)
            {
                current_node->stored_node_count += 1;
                if (value < current_node->stored_value_range.min_value)
                {
                    current_node->stored_value_range.min_value = value;
                }
                if (value > current_node->stored_value_range.max_value)
                {
                    current_node->stored_value_range.max_value = value;
                }
            }

            return inserted;


        } else {
            // We should go to the left
            // If there is no left child, that means this is exactly where
            // the new node should be inserted
            if (current_node->left == nullptr) {
                current_node->left = new Node;
                current_node->left->value = value;

                current_node->left->stored_node_count = 1;
                current_node->left->stored_value_range = {value,value};

                current_node->stored_node_count += 1;

                if ( value < current_node->stored_value_range.min_value )
                {
                    current_node->stored_value_range.min_value = value;
                }

                if (value > current_node->stored_value_range.max_value)
                {
                    current_node->stored_value_range.max_value = value;
                }

                return true;
            }
            // Otherwise, if there is a left child, continue there
            //return _insert_helper(current_node->left, value);
            bool inserted = _insert_helper(current_node->left, value);
            if (inserted)
            {
                current_node->stored_node_count += 1;

                if (value < current_node->stored_value_range.min_value)
                {
                    current_node->stored_value_range.min_value = value;
                }
                if (value > current_node->stored_value_range.max_value)
                {
                    current_node->stored_value_range.max_value = value;
                }
            }

            return inserted;
        }
    }


    int count_lt(int x)
    {
        // TASK 3 - How many nodes have a value less than x?
        // Your answer should have a complexity of O(h), where h is the height of the tree, and should depend on your implementation of task 2.
        Node* current_node = root;
        int count = 0;

        while (current_node != nullptr)
        {
            if (current_node->value < x)
            {
                count++;

                if (current_node->left != nullptr)
                {
                    count += current_node->left->stored_node_count;
                }

                current_node = current_node->right;
            }
            else
            {
                current_node = current_node->left;
            }
        }
            return count;
    }

    int count_range(int x, int y) {
        // TASK 4 - Count of nodes with value v satisfying x <= v <= y
        // Your answer should have a complexity of O(h), where h is the height of the tree
        Node* current_node = root;
        int count_less_than = 0;

        while (current_node != nullptr)
        {
            if (current_node->value <= y)
            {
                count_less_than++;

                if (current_node->left != nullptr)
                {
                    count_less_than += current_node->left->stored_node_count;
                }

                current_node = current_node->right;
            }
            else
            {
                current_node = current_node->left;
            }
        }

        current_node = root;
        int count_greater_than = 0;

        while (current_node != nullptr)
        {
            if (current_node->value >= x)
            {
                count_greater_than++;

                if (current_node->right != nullptr)
                {
                    count_greater_than += current_node->right->stored_node_count;
                }

                current_node = current_node->left;
            }
            else
            {
                current_node = current_node->right;
            }
        }

        return count_less_than+count_greater_than - root->stored_node_count;
    }
};

int main(int argc, char** argv) {
    if (argc == 1) {

        BinarySearchTree bst;
        bst.insert(50);
        bst.insert(30);
        bst.insert(70);
        bst.insert(20);
        bst.insert(40);
        bst.insert(60);
        bst.insert(80);

        cout << bst.search(40)->value << endl; // Should print 40

        try {
            bst.insert(40); // Should throw an error
        } catch (const char* msg) {
            cout << msg << endl; // Should print "Value already present in BST"
        }

        cout << "=== TASK 1 ===" << endl;
        cout << "Node Count: " << bst.root->node_count() << ", Range: " << bst.root->value_range() << endl;
        cout << "Left Count: " << bst.root->left->node_count() << ", Range: " << bst.root->left->value_range() << endl;
        cout << "Right Count: " << bst.root->right->node_count() << ", Range: " << bst.root->right->value_range() << endl;

        cout << "=== TASK 2 ===" << endl;
        cout << "Node Count: " << bst.root->stored_node_count << ", Range: " << bst.root->stored_value_range << endl;
        cout << "Left Count: " << bst.root->left->stored_node_count << ", Range: " << bst.root->left->stored_value_range << endl;
        cout << "Right Count: " << bst.root->right->stored_node_count << ", Range: " << bst.root->right->stored_value_range << endl;

        cout << "=== TASK 3 ===" << endl;
        cout << "Amount Less Than 50: " << bst.count_lt(50) << endl;
        cout << "Amount Less Than 35: " << bst.count_lt(35) << endl;
        cout << "Amount Less Than 65: " << bst.count_lt(65) << endl;

        cout << "=== TASK 4 ===" << endl;
        cout << "Amount between 0 and 50: " << bst.count_range(0, 50) << endl;
        cout << "Amount between 25 and 65: " << bst.count_range(25, 65) << endl;
        cout << "Amount between 70 and 70: " << bst.count_range(70, 70) << endl;
        cout << "Amount between 30 and 70: " << bst.count_range(30, 70) << endl;

        return 0;
    } else {
        // Ontrack test cases DO NOT MODIFY
        int command = 0;
        BinarySearchTree bst;
        Node* context_node = nullptr;
        string menu = "0. Show this menu\n1. Insert a value into bst\n2. Search for a node\n3. Call node_count\n4. Return stored_node_count\n5. Call value_range\n6. Return stored_value_range\n7. Call count_lt\n8. Call count_range\n9. Exit";
        int value;
        while (command != 9) {
            try {
                if (command == 0) {
                    cout << menu << endl;
                } else if (command == 1) {
                    cout << "Enter value to insert: " << endl;
                    cin >> value;
                    bst.insert(value);
                } else if (command == 2) {
                    cout << "Enter value to search for: "<< endl;
                    cin >> value;
                    context_node = bst.search(value);
                    if (context_node) {
                        cout << "Found node with " << context_node->value << endl;
                    } else {
                        cout << "Could not find node with value " << value << endl;
                    }
                } else if (command == 3) {
                    if (context_node) {
                        cout << "Node count called as " << context_node->node_count() << endl;
                    } else throw "ERROR: Context node is null";
                } else if (command == 4) {
                    if (context_node) {
                        cout << "Node count stored as " << context_node->stored_node_count << endl;
                    } else throw "ERROR: Context node is null";
                } else if (command == 5) {
                    if (context_node) {
                        cout << "Value range called as " << context_node->value_range() << endl;
                    } else throw "ERROR: Context node is null";
                } else if (command == 6) {
                    if (context_node) {
                        cout << "Value range stored as " << context_node->stored_value_range << endl;
                    } else throw "ERROR: Context node is null";
                } else if (command == 7) {
                    cout << "Enter upper bound: " << endl;
                    cin >> value;
                    cout << "Amount less than " << value << " is " << bst.count_lt(value) << endl;;
                } else if (command == 8) {
                    cout << "Enter lower bound: " << endl;
                    cin >> value;
                    int ub;
                    cout << "Enter upper bound: " << endl;
                    cin >> ub;
                    cout << "Number of nodes between " << value << " and " << ub << " is " << bst.count_range(value, ub) << endl;
                }

            } catch (const char* msg) {
                cout << msg << endl;
            }
            cout << "Enter command (0-9): " << endl;
            if (!(cin >> command)) {
                cout << "ERROR: Could read next command" << endl;
                return 1;
            };
        }
        return 0;
    }
}