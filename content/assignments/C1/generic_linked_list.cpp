#include <iostream>
#include <string>
using namespace std;

template<typename T>
struct Node {
    // ...
    T item;
    Node<T>* next;
};

template<typename T>
struct LinkedList {
    Node<T>* first_node = nullptr;

    int size = 0;

    // TODO: determine what should the input of the method be and implement it
    void append(T item) {
        // ...
        Node<T>* new_node = new Node<T>;
        new_node->item = item;
        new_node->next = nullptr;

        if (first_node == nullptr)
        {
            first_node = new_node;
            size++;
            return;
        }

        Node<T>* current_node = first_node;
        while (current_node->next != nullptr)
        {
            current_node = current_node->next;
        }

        current_node->next = new_node;
        size++;
        return;
    }

    Node<T>* get_node_at_index(int index) {
        // ...
        Node<T>* current_node = first_node;
        for (int i = 0; i < index; i++)
        {
            current_node = current_node->next;
        }
        return current_node;
    }

    // TODO: determine what should the input of the method be and implement it
    void set_index(int index, T item) {
        // ...
        Node<T>* current_node = get_node_at_index(index);
        current_node->item = item;
        return;
    }

    void display() {
        // This method is not marked (it's optional)
        // But it will be very helpful for your debugging to have a method that
        // prints what's in the linked list currently.
        Node<T>* current_node = first_node;
        while (current_node != nullptr)
        {
            cout << current_node->item << " ";
            current_node = current_node->next;
        }
        cout << endl;
        return;
    }
};


/// Test the linked list implementation
int main() {
    LinkedList<int> list;
    list.append(10);
    list.append(20);
    list.append(30);

    cout << "Linked List: ";
    list.display();

    list.set_index(1, 25);
    cout << "Linked List after setting index 1 to 25: ";
    list.display();

    cout << "Item at index 2: " << list.get_node_at_index(2)->item << endl;

    LinkedList<string> list_string;
    list_string.append("Apple");
    list_string.append("Banana");
    list_string.append("Cat");

    cout << "Linked List: ";
    list_string.display();

    list_string.set_index(1, "Peach");
    cout << "Linked List after setting index 1 to Peach: ";
    list_string.display();

    cout << "Item at index 2: " << list_string .get_node_at_index(2)->item << endl;

    LinkedList<double> list_double;
    list_double.append(10.0);
    list_double.append(11.0);
    list_double.append(12.0);

    cout << "Linked List: ";
    list_double.display();

    list_double.set_index(1, 11.5);
    cout << "Linked List after setting index 1 to 11.5: ";
    list_double.display();

    cout << "Item at index 2: " << list_double .get_node_at_index(2)->item << endl;





    return 0;
}

