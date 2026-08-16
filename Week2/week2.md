# Week 2
## 1. Custom Types
```C++
struct User {
    string username;
    string password;
    int created_at;
    bool banned;
};
```

### the difference between Class and Struct:
class:
```C++
class Student {
private:
    string name;
    int age;
};
```

struct:
```C++
struct Student {
public:
    string name;
    int age;
};
```
## 2. Pointers
```txt
"*": pointer declaration or dereference
"&": address-of operator
"->"：access a member through a pointer
```

For example:
```C++
int x= 10;
int* p = &x;
```

```C++
struct User
{
    int id;
};

int main()
{
    User user1;
    user1.id = 1;
    User* user1_add = &user1;
    
    if ((*user1_add).id == (user1_add->id))
    {
        cout<< "Ture" << endl;
    }
    
    return 0;
}
```

`p->value` is equivalent to `(*p).value`


So you could remember:
```txt
&x          → get the address of x

int* p      → declare p as a pointer

*p          → get the value/object pointed to by p

object.x    → access a member of an object

pointer->x  → access a member of the object pointed to by a pointer
```

## 3. Linked List
```C++
#include <iostrem>
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
    int size;

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

        current->next = &new_node;
        size++;
    }
};
```

## 4. Original Object and Copy
In C++, whether something is a copy or refers to the original object mainly depends on how the varible is declared.

```C++
int b = a;
```
This creates a copy of a. a and b are two seprate variables.

```C++
int& b = a;
```
This creates a reference to a. b is another name for the same variable, so changing b also changes a.

For pointers:
```C++
Node* current = first;
```
This copies the address stored in first into current. 
first and current are two separate pointer variables, 
but they initially point to the same Node.

Therefore:
```C++
current = current->next;
```
only changes the address stored in the current. It doesn't change first;

However,
```C++
current->item = 10;
```
changes the actual Node that currently points to. Since first may point to the same Node,
the change could also be observed through first;

A useful rule is:
```txt
T x= y ; usually creates a copy
T& x=y ; creates a reference to the original object
T* x=y ; copies a pointer/address, not object itself;
```

for example:
```C++
Node current = *first;   // copy of the Node
Node& current = *first;  // reference to the original Node
Node* current = first;   // copy of the pointer/address
```

The key distinction is copying an object vs copying an address vs referring to the original object.

## 5. Dangling Pointer
A dangling pointer is a pointer that stores the address of object whose lifetime has already ended.

Example:
```C++
int cool_func()
{
    int my_int = 10;
    return &my_int;
}
```
my_int is a local variable. Its lifetime ends when cool_func() finishes.
Therefore, the returned pointer points to memory that is no longer valid
for my_int.

## 6. Stack
Local variables are usually stored in stack memory
```C++
void func()
{
    int x = 10;
}
```
x only exists during the execution of func().
When the function finishes, the lifetime of x ends and its stack memory may be
refused by another function.

Therefore, returning the address of a local variable is unsafe:
```C++
int* func()
{
    int x=10;
    return &x; // Wrong
}
```
## 7. Heap
Heap memory could be allocated dynamically using new.
```C++
int* num = new int;
*num = 6;
```

new int creates an integer in the heap and returns its address.
```C++
num
 |
 v
Heap
+-----+
|  6  |
+-----+
```
The heap object do not disappear when function ends.

## 8. Stack Pointer vs. Heap Object
```C++
int* my_int = new int;
```
There are two different things:
- my_int is a local pointer variable
- new int creates an integer object in the heap

When the function finishes:
- my_int disappear.
- The object still exists.

Example:
```C++
int* cool_fuc()
{
    int* my_int = new int;
    *my_int = 10;
    return my_int;
}
```
The returned address still points to a valid heap object.

## 9. `new int()` vs `new int`
`new int` allocates memory for an integer on the heap, but the integer 
id not initialized.

```C++
int* a= new int();
```
The value of a is indeterminate.

new int() also allocates memory on the heap,
but it value-initializes the integer to 0;

So:
```C++
*b == 0;
```

In short:
- new int -> allocates an uninitialized integer
- new int() -> allocates an integer initialized to 0

## 10. Memory Management 
Heap memory created with new should eventually be released using delete.
```C++
delete pointer;
```
Otherwise, the program may cause a memory leak.

## 11. Memory Leak
A memory leak happens when memory is allocated on the heap using new, 
but that memory is never released with delete and the program loses access to it.

For example:
```C++
int* p = new int;
*p = 10;

p = nullptr;
```

The integer created by new int still exists in heap memory, but p no longer 
points to it.
```txt
Before:

p
│
▼
Heap
┌────┐
│ 10 │
└────┘


After p = nullptr:

p → nullptr

Heap
┌────┐
│ 10 │   ← still allocated
└────┘
```
Since there is no pointer left to access that heap object, the allocated
memory cannot be released later. This is called a memory leak.

The correct approach is:
```C++
delete p;
p = nullptr;
```

You can think of it as :
new -> allocates memory from the heap
delete -> releases that memory from the heap to the system

For example, this causes a memory leak:
```C++
while (true)
{
    int* p = new int;
}
```

Each iteration creates a new integer on the heap. The local pointer p disappears at the end of the iteration, 
but the heap object remains allocated.

**Memory leak VS Dangling pointer**
A useful distinction is:
- Memory Leak: the object still exists in heap memory, but no pointer could access it.
- Dangling Pointer: the pointer still exists, but the object it points to no longer exists.

