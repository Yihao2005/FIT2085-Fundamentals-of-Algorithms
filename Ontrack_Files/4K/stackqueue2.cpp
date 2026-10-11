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

/*
 * The first queue is represented by a FIFO queue, preserving the students' original order.
 * The second queue is represented by a stack because students are added to and removed from its
 * front. Therefore, the most recently transferred student is the first one available for check-in.
 *
 * The algorithm only checks in the next required student. Whenever that student is inaccessible, transfers are
 * forced. Checking in an accessible target is safe. Therefore, reaching checked == n gives a valid complete check-in;
 * otherwise, the algorithm stops in a state where completion is impossible.
 *
 * Each student enter and leave the first queue at most once, and is pushed onto and popped from the stack at most once.
 * Each operation tasks O(1) time, so the total time complexity is O(n).
 */
int main()
{
    int n;
    cin >> n;
    Stack stack;
    Queue queue;

    int checked = 0;

    // Add all students into the first queue
    for (int i = 0; i < n; i++)
    {
        long long val;
        cin >> val;
        queue.enqueue(val);
    }

    // At the start of iteration i, students 1 through i - 1 have
    // checked in in the required order. Student i is the next target
    for (int i = 1; i <= n; i++)
    {
        // If neither accessible student is i, no check-in is permitted.
        // The only possible move is to transfer the front student
        // of the first queue to stack. Repeat until i becomes
        // accessible or the first queue becomes empty.
        while (
            queue.front != nullptr && // The queue is not empty
            queue.peak() != i && // The front of queue is not i.
            (stack.top == nullptr || stack.peak() != i) // The stack is empty or the top of stack is not i.
        )
        {
            stack.push(queue.dequeue());
        }

        if (stack.top != nullptr && stack.peak() == i)
        {
            // Student i must check in before every remaining student.
            // Transferring more students would only bury i in the stack,
            // so checking i in immediately is safe.
            // It only removes the student required and exposes the next stack item.
            stack.pop();
            checked++;
        }
        else if (queue.front != nullptr && queue.peak() == i)
        {
            // Student i must check in before every remaining student.
            // Moving i to the stack and immediately popping it would have
            // the same effect as checking it in directly, so this is safe.
            // It only removes the student required and exposes the next queue item.
            queue.dequeue();
            checked++;
        }
        else
        {
            // The first queue is empty and the stack top is not i.
            // Student i is blocked inside the stack. The blocking students before i could not
            // check-in, so complete check-in is impossible.
            break;
        }
    }

    string result = (checked == n ? "YES" : "NO");
    cout << result << endl;
    return 0;
}