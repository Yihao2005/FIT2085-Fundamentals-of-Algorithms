#include <iostream>
#include <string>

using namespace std;

struct Node
{
    int item;
    Node* next;
};

struct LinkedList
{
    Node* first = nullptr;
    int size=0;

    void append(int item)
    {
        Node* new_node = new Node;
        new_node->item = item;
        new_node->next = nullptr;

        if(first == nullptr)
        {
            first = new_node;
            size++;
            return;
        }

        Node* current = first;

        while (current->next != nullptr)
        {
            current = current->next;
        }

        current->next = new_node;
        size++;
    }

    void display()
    {
        Node* current = first;
        for (int i = 0; i < size; i++)
        {
            cout << current->item << " ";
            current = current->next;
        }

    }
};

int main()
{
    LinkedList list;
    list.append(1);
    list.append(2);
    list.append(3);
    list.display();

}