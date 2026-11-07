#include <iostream>
#include <string>

using namespace std;

struct Node {
    Node* next = nullptr;
    int value;
};

struct StrNode {
    StrNode* next = nullptr;
    string value;
};

struct Stack {
    Node* top = nullptr;
    int size = 0;

    void push(int value) {
        Node* new_node = new Node;
        new_node->value = value;
        new_node->next = top;
        top = new_node;
        size++;
    }

    int peek() {
        if (top == nullptr) {
            throw "Stack is empty";
        }
        return top->value;
    }

    int pop() {
        if (top == nullptr) {
            throw "Stack is empty";
        }
        int result = top->value;
        Node* new_top = top->next;
        delete top;
        top = new_top;
        size--;
        return result;
    }

    // TASK 1
    int current_size() {
        // TODO
        return size;
    }
};

struct LinkedQueue {
    // TASK 2: Implement the linked queue class.
    // You should be able to use one of the Node structs above.
    // Recall that a queue requires two pointers, a `rear` and a `front`.
    StrNode* front = nullptr;
    StrNode* rear = nullptr;
    int size = 0;

    void enqueue(string item) {
        StrNode* new_node = new StrNode;
        new_node->value = item;
        new_node->next = nullptr;

        size++;

        if (is_empty())
        {
            front = new_node;
            rear = new_node;
        } else
        {
            rear->next = new_node;
            rear = new_node;
        }

    }

    string dequeue() {
        if (is_empty())
        {
            throw "Queue is empty";
        }

        StrNode* next_front = front->next;
        string result = front->value;
        delete front;
        front = next_front;
        size--;

        if (is_empty())
        {
            rear = nullptr;
        }
        return result;


    }

    string peek() {
        if (is_empty())
        {
            throw "Queue is empty";
        }
        string result = front->value;
        return result;
    }

    bool is_empty() {
        return front == nullptr;
    }

};

int main() {
    Stack stack;
    LinkedQueue queue;

    string menu = "1. Push a value onto the stack\n2. Pop the top value off the stack\n3. Peek at the top of the stack\n4. Enqueue a value into the queue\n5. Dequeue the front value from the queue\n6. Peek at the front of the queue\n7. See the current stack size\n8. See the menu again\n9. Quit";

    cout << menu << endl;
    int command = 0;
    cout << "Enter command (1-9):" << endl;
    cin >> command;

    while (command != 9) {
        // The stack/queue methods throw when they are empty, so we wrap
        // each action in a try/catch and report the error instead of crashing.
        try {
            if (command == 1) {
                cout << "Enter the value to push:" << endl;
                int value;
                cin >> value;
                stack.push(value);
            } else if (command == 2) {
                cout << "Popped: " << stack.pop() << endl;
            } else if (command == 3) {
                cout << "Top of stack: " << stack.peek() << endl;
            } else if (command == 4) {
                cout << "Enter the value to enqueue:" << endl;
                string value;
                cin >> value;
                queue.enqueue(value);
            } else if (command == 5) {
                cout << "Dequeued: " << queue.dequeue() << endl;
            } else if (command == 6) {
                cout << "Front of queue: " << queue.peek() << endl;
            } else if (command == 7) {
                cout << "Stack size: " << stack.current_size() << endl;
            } else if (command == 8) {
                cout << menu << endl;
            } else {
                cout << "Unknown command." << endl;
            }
        } catch (const char* error) {
            cout << "Error: " << error << endl;
        }

        cout << "Enter command (1-9):" << endl;
        cin >> command;
    }

    return 0;
}