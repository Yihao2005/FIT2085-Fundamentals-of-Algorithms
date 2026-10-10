#include <iostream>
#include <string>

using namespace std;

struct Node
{
    int value;
    Node* next = nullptr;
    Node* last = nullptr;
};

struct Stack
{
    Node* top = nullptr;

    void push(int value){
        Node* new_node = new Node();
        new_node->value = value;
        new_node->next = top;
        top = new_node;
    }

    int pop()
    {
        if (top == nullptr)
        {
            throw "The stack is empty";
        }
        Node* new_top = top->next;
        int result = top->value;
        delete top;
        top = new_top;
        return result;
    }

    int peak()
    {
        if (top == nullptr) throw "The stack is empty";
        return top->value;
    }

    ~Stack()
    {
        while (top != nullptr) pop();
    }
};

struct Queue
{
    Node* front = nullptr;
    Node* rear = nullptr;

    void enqueue(int value)
    {
        Node* new_node = new Node();
        new_node->value = value;

        if (front == nullptr)
        {
            front = new_node;
            rear = front;
            return;
        }

        rear->next = new_node;
        new_node->last = rear;

        rear = new_node;
    }

    int dequeue()
    {
        if (front == nullptr)
        {
            throw "The queue is empty";
        }

        Node* old_front = front;
        int result = old_front->value;

        front = front->next;

        if (front != nullptr)
        {
            front->last = nullptr;
        }
        else
        {
            rear = nullptr;
        }

        delete old_front;
        return result;
    }

    int peak()
    {
        if (front == nullptr) throw "The queue is empty";
        return front->value;
    }

    ~Queue()
    {
        while (front != nullptr) dequeue();
    }
};

int main()
{
    int n;
    cin >> n;
    Stack stack;
    Queue queue;

    int checked = 0;

    for (int i = 0; i < n; i++)
    {
        long long val;
        cin >> val;
        queue.enqueue(val);
    }

    for (int i = 1; i <= n; i++)
    {
        while (
            queue.front != nullptr &&
            queue.peak() != i &&
            (stack.top == nullptr || stack.peak() != i)
        )
        {
            stack.push(queue.dequeue());
        }

        if (stack.top != nullptr && stack.peak() == i)
        {
            stack.pop();
            checked++;
        }
        else if (queue.front != nullptr && queue.peak() == i)
        {
            queue.dequeue();
            checked++;
        }
        else
        {
            break;
        }
    }

    string result = (checked == n ? "YES" : "NO");
    cout << result << endl;
    return 0;
}