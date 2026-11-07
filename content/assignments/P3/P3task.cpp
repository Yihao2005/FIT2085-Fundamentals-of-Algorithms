#include <iostream>
#include <string>

using namespace std;

const int ARRAY_SIZE = 100;

struct Node {
    // Implement as you need
    int arr[ARRAY_SIZE];
    Node* next;
};

struct PointingLinkedArrays {
    Node* first_node = nullptr;
    int size = 0;
    int node_size = 0;
    int get_size() {
        // ...
        return size;
    }

    // O(n): Each nodes stores 100 integers, so there are (n/100 + 1) nodes.
    // If we want to append the array, we need to travel from the first node to the
    // last node. Therefore, the time complexity is Linear.
    void append(int value) {
        // ...
        if (size%ARRAY_SIZE == 0)
        {
            Node* new_node = new Node();
            new_node->arr[0] = value;
            new_node->next = nullptr;
            if (first_node == nullptr)
            {
                first_node = new_node;
            }else
            {
                Node* current_node = first_node;
                for (int i = node_size-1; i>0; i--)
                {
                    current_node = current_node->next;
                }
                current_node->next = new_node;
            }
            size++;
            node_size++;
            return;
        }
        Node* current_node = first_node;
        for (int i = node_size-1; i > 0; i--)
        {
            current_node = current_node->next;
        }
        current_node->arr[size%ARRAY_SIZE] = value;
        size++;

    }

    // O(n): If we want to get to the index we want, we need to travel from the first
    // node to the (index/100 + 1)th node. Therefore, the time is directly proportional to
    // the index.
    int get_index(int index) {
        // ...
        int node_number = index/ARRAY_SIZE;
        int index_node = index%ARRAY_SIZE;
        Node* current_node = first_node;
        for (int i = node_number; i > 0; i--)
        {
            current_node = current_node->next;
        }
        return current_node->arr[index_node];
    }
    void set_index(int index, int new_value) {
        // ...
        int node_number = index/ARRAY_SIZE;
        int index_node = index%ARRAY_SIZE;
        Node* current_node = first_node;
        for (int i = node_number; i > 0; i--)
        {
            current_node = current_node->next;
        }
        current_node->arr[index_node] = new_value;
    }
};

int main() {
    PointingLinkedArrays list;
    for (int i=0;i <200; i++)
    {
        list.append(i);
    }


    string menu = "1. Append a new item\n2. Get value of an index\n3. Set value of an index\n4. See the current size\n5. See the menu again\n6. Quit";
    int command = 5;

    while (command != 6) {
        if (command == 1) {
            cout << "Enter the value to append:" << endl;
            int new_val;
            cin >> new_val;
            list.append(new_val);
        } else if (command == 2) {
            cout << "Enter the index:" << endl;
            int index;
            cin >> index;
            cout << "Value at index " << index << ": " << list.get_index(index) << endl;
        } else if (command == 3) {
            cout << "Enter the index:" << endl;
            int index;
            cin >> index;
            cout << "Enter the new value:" << endl;
            int new_value;
            cin >> new_value;
            list.set_index(index, new_value);
        } else if (command == 4) {
            cout << "Current size: " << list.get_size() << endl;
        } else if (command == 5) {
            cout << menu << endl;
        }

        cout << "Enter command (1-6):" << endl;
        cin >> command;
    }

    return 0;
}