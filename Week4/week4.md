# More Pointers, Stack and Queue

## 1. Heap vs Stack Memory

C++ programs mainly use two areas of memory: the **stack** and the **heap**.

### Stack

The stack is mainly used for local variables.

```cpp
void example() {
    int x = 10;
    int arr[100];
}
```

Both `x` and `arr` are stored on the stack.

Important properties:

- Memory is managed automatically.
- Local variables are removed when the function finishes.
- Stack memory is relatively limited.
- The compiler generally needs to know the required stack memory size.

For example:

```cpp
int n;
cin >> n;

// Not appropriate in standard C++ for a runtime-sized array
int arr[n];
```

The size `n` is only known at runtime.

### Heap

The heap is used for dynamically allocated memory.

```cpp
int* x = new int;
```

`new` allocates memory on the heap and returns its address.

Unlike stack memory, heap memory must normally be managed manually:

```cpp
int* x = new int;

*x = 10;

delete x;
```

If dynamically allocated memory is never deleted, it can cause a **memory leak**.

---

## 2. Arrays on the Heap

A normal fixed-size array can be created on the stack:

```cpp
int arr[100];
```

A dynamic array can be created on the heap:

```cpp
int* arr = new int[100];
```

`new int[100]` creates a block containing 100 integers and returns the address of the first integer.

Therefore:

```cpp
arr
```

is a pointer to the first element.

We can still use normal array syntax:

```cpp
arr[0] = 10;
arr[1] = 20;
arr[2] = 30;
```

When finished:

```cpp
delete[] arr;
```

Arrays must use:

```cpp
delete[]
```

instead of:

```cpp
delete
```

---

## 3. Array Syntax and Pointer Arithmetic

Suppose:

```cpp
int* arr = new int[10];
```

These expressions are equivalent:

```cpp
arr[0]
*arr
```

Similarly:

```cpp
arr[5]
*(arr + 5)
```

and:

```cpp
arr[8] = 8;
*(arr + 8) = 8;
```

Therefore:

```cpp
arr[i]
```

is essentially equivalent to:

```cpp
*(arr + i)
```

Conceptually:

```text
arr
 |
 v
+----+----+----+----+----+----+
| 0  | 1  | 2  | 3  | 4  | 5  |
+----+----+----+----+----+----+
                           ^
                           |
                        arr + 5
```

Because `arr` is an `int*`, `arr + 5` means:

> move forward by 5 integers

not 5 bytes.

---

## 4. Accessing Outside an Array

C++ normally does not perform automatic array bounds checking.

For example:

```cpp
int* arr = new int[10];

arr[12] = 12;
```

This is invalid because valid indexes are only:

```text
0 to 9
```

However, the program may still appear to work.

This happens because:

```cpp
arr[12]
```

simply accesses memory 12 integer positions after `arr`.

This is **undefined behaviour**.

The program may:

- appear to work,
- overwrite another variable,
- corrupt memory,
- crash,
- behave differently later.

Therefore, accessing outside allocated memory must always be avoided.

---

# 5. Dynamic 2D Arrays

A normal 2D array can be created on the stack:

```cpp
int arr[10][15];
```

For a dynamically sized 2D array, we can allocate it on the heap.

Suppose we want:

```text
10 rows
15 columns
```

First create an array of integer pointers:

```cpp
int** arr = new int*[10];
```

Then create one integer array for every row:

```cpp
for (int i = 0; i < 10; i++) {
    arr[i] = new int[15];
}
```

The memory structure is conceptually:

```text
arr
 |
 v
+-------+
| int*  | ----> [int][int][int]...[15]
+-------+
| int*  | ----> [int][int][int]...[15]
+-------+
| int*  | ----> [int][int][int]...[15]
+-------+
|  ...  |
+-------+
```

We can access elements normally:

```cpp
arr[5][2] = 45;
```

---

## 6. Deleting a 2D Heap Array

Every inner array was allocated separately:

```cpp
arr[i] = new int[15];
```

Therefore, each inner array must first be deleted:

```cpp
for (int i = 0; i < 10; i++) {
    delete[] arr[i];
}
```

Then delete the outer array:

```cpp
delete[] arr;
```

The order matters.

Do:

```cpp
for (int i = 0; i < rows; i++) {
    delete[] arr[i];
}

delete[] arr;
```

Not:

```cpp
delete[] arr;

// Too late: arr[i] can no longer safely be accessed
```

---

# 7. Dynamic 2D Array Example

Read an `n × n` matrix from the user:

```cpp
int n;
cin >> n;

int** arr = new int*[n];

for (int i = 0; i < n; i++) {
    arr[i] = new int[n];
}
```

Read values:

```cpp
for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
        cin >> arr[i][j];
    }
}
```

Delete memory:

```cpp
for (int i = 0; i < n; i++) {
    delete[] arr[i];
}

delete[] arr;
```

---

# 8. Constants

A constant is a value that should not change.

```cpp
const int PASS_THRESHOLD = 50;
```

Trying to modify it causes a compile-time error:

```cpp
PASS_THRESHOLD = 60; // Error
```

Constants improve readability and reduce accidental modification.

Instead of:

```cpp
if (mark >= 50) {
```

use:

```cpp
const int PASS_THRESHOLD = 50;

if (mark >= PASS_THRESHOLD) {
```

This communicates what `50` means.

---

# 9. Code Should Be Written for Humans

Correctness is not the only goal of software development.

Good code should also be:

- readable,
- understandable,
- maintainable,
- extendable,
- difficult to misuse.

For example:

```cpp
int a;
int b;
```

is less understandable than:

```cpp
int rear;
int size;
```

Good variable names communicate the design of the program.

This is also why choosing an appropriate data structure is important.

If a program only needs access to specific parts of a collection, exposing every possible array operation may make the program harder to understand and easier to misuse.

---

# 10. Stack Data Structure

A **stack** follows:

```text
LIFO
Last In, First Out
```

The last item inserted is the first item removed.

A real-world analogy is a stack of books.

```text
push 1
push 2
push 3

Top
 |
 v
+---+
| 3 |
+---+
| 2 |
+---+
| 1 |
+---+
```

If we remove one item:

```text
pop() -> 3
```

---

## 11. Main Stack Operations

The three important stack operations are:

### `push`

Add an item to the top.

```cpp
stack.push(10);
```

### `peek`

Look at the top item without removing it.

```cpp
stack.peek();
```

### `pop`

Return and remove the top item.

```cpp
stack.pop();
```

---

# 12. Linked Stack Implementation

A stack can be implemented using linked nodes.

```cpp
struct Node {
    Node* next = nullptr;
    int value;
};
```

The stack stores a pointer to its top node:

```cpp
struct Stack {
    Node* top = nullptr;
};
```

---

## 13. Stack `push`

```cpp
void push(int value) {
    Node* new_node = new Node;

    new_node->value = value;
    new_node->next = top;

    top = new_node;
}
```

Suppose initially:

```text
top
 |
 v
[3] -> [2] -> [1] -> nullptr
```

After:

```cpp
push(4);
```

we create:

```text
[4]
```

then:

```cpp
new_node->next = top;
```

produces:

```text
[4] -> [3] -> [2] -> [1]
```

and:

```cpp
top = new_node;
```

makes `4` the new top.

---

# 14. Stack `peek`

```cpp
int peek() {
    if (top == nullptr) {
        throw "Stack is empty";
    }

    return top->value;
}
```

It only reads the top value.

Nothing is removed.

---

# 15. Stack `pop`

```cpp
int pop() {
    if (top == nullptr) {
        throw "Stack is empty";
    }

    int result = top->value;

    Node* new_top = top->next;

    delete top;

    top = new_top;

    return result;
}
```

Suppose:

```text
top
 |
 v
[3] -> [2] -> [1]
```

First:

```cpp
Node* new_top = top->next;
```

Now:

```text
top ------> [3]
              |
              v
new_top ---> [2] -> [1]
```

Then:

```cpp
delete top;
```

removes `[3]`.

Finally:

```cpp
top = new_top;
```

produces:

```text
top
 |
 v
[2] -> [1]
```

---

# 16. Stack Time Complexity

For a linked stack:

| Operation | Time Complexity |
|---|---:|
| `push()` | O(1) |
| `peek()` | O(1) |
| `pop()` | O(1) |

This is because every operation only accesses the node at the top.

There is no traversal.

---

# 17. Why Stack Memory Is Called a Stack

The stack memory area follows a similar LIFO principle.

When functions are called:

```text
main()
  ↓
functionA()
  ↓
functionB()
```

their stack frames conceptually behave like:

```text
TOP
+-------------+
| functionB  |
+-------------+
| functionA  |
+-------------+
| main       |
+-------------+
```

When `functionB()` finishes, its stack frame is removed first.

Then `functionA()`.

Therefore:

```text
Last function called
        ↓
First function returned
```

This resembles the LIFO stack data structure.

---

# 18. Queue Data Structure

A **queue** follows:

```text
FIFO
First In, First Out
```

The first item inserted is the first item removed.

Example:

```text
enqueue A
enqueue B
enqueue C

FRONT                 REAR
  |                     |
  v                     v
[A] [B] [C]
```

Calling:

```cpp
dequeue();
```

returns:

```text
A
```

---

# 19. Queue Operations

Important queue operations are:

### `enqueue`

Add an item to the rear.

```cpp
queue.enqueue("A");
```

### `peek`

View the front item without removing it.

```cpp
queue.peek();
```

### `dequeue`

Return and remove the front item.

```cpp
queue.dequeue();
```

---

# 20. Array-Based Queue

A basic queue can use a dynamic array:

```cpp
struct Queue {
    int current_capacity = 10;
    string* array = new string[current_capacity];

    int rear = 0;
    int size = 0;
};
```

Here:

```text
rear
```

means:

> the position where the next item will be inserted.

And:

```text
size
```

means:

> how many active items are currently inside the queue.

The front item can therefore be found using:

```cpp
rear - size
```

---

# 21. Basic Queue Enqueue

```cpp
array[rear] = item;
rear++;
size++;
```

Example:

```text
Before:

rear = 2
size = 2

[ A ][ B ][   ][   ]
            ^
            rear
```

After:

```cpp
enqueue("C");
```

we get:

```text
[ A ][ B ][ C ][   ]
                 ^
                 rear
```

---

# 22. Dynamic Resizing

If the array becomes full:

```cpp
if (current_capacity == rear) {
```

create a larger array:

```cpp
string* new_array =
    new string[current_capacity * 2];
```

Copy the items:

```cpp
for (int i = 0; i < current_capacity; i++) {
    new_array[i] = array[i];
}
```

Delete the old array:

```cpp
delete[] array;
```

Then:

```cpp
array = new_array;
current_capacity *= 2;
```

---

# 23. Problem with the Basic Queue

Suppose the array contains:

```text
[A][B][C][D][E]
```

After several dequeues:

```text
[x][x][x][D][E]
```

The old positions at the front are no longer needed.

However, if `rear` always moves right:

```text
[x][x][x][D][E][ ][ ]
                       →
```

the queue eventually reaches the end of the array even though there may be unused space at the beginning.

Moving all remaining elements left after every `dequeue()` would solve the memory problem, but would cost:

```text
O(n)
```

per dequeue.

A better solution is a **circular queue**.

---

# 24. Circular Queue

A circular queue treats the array as if the end connects back to the beginning.

Instead of:

```text
0 1 2 3 4 5
-----------> END
```

think:

```text
0 → 1 → 2 → 3 → 4 → 5
↑                   ↓
└───────────────────┘
```

When `rear` reaches the end:

```cpp
rear++;
```

we wrap around:

```cpp
if (rear == current_capacity) {
    rear = 0;
}
```

An equivalent expression is often:

```cpp
rear = (rear + 1) % current_capacity;
```

---

# 25. Finding the Front of a Circular Queue

A simple formula is:

```cpp
(rear - size + current_capacity) % current_capacity
```

Therefore:

```cpp
int front =
    (rear - size + current_capacity)
    % current_capacity;
```

Why add `current_capacity`?

Because:

```cpp
rear - size
```

may become negative after wrapping.

---

# 26. Circular Queue Example

Suppose:

```text
capacity = 5
rear = 2
size = 4
```

Then:

```cpp
front = (2 - 4 + 5) % 5;
```

so:

```text
front = 3
```

The active queue may look like:

```text
index
 0   1   2   3   4

[C] [D] [ ] [A] [B]
         ^
         rear

front = 3
```

Logical order:

```text
A → B → C → D
```

even though physically the values wrap around the array.

---

# 27. Queue Time Complexity

For the circular queue:

| Operation | Best | Worst |
|---|---:|---:|
| `peek()` | O(1) | O(1) |
| `dequeue()` | O(1) | O(1) |
| `enqueue()` | O(1) | O(n) |

`enqueue()` is usually:

```text
O(1)
```

because it simply stores one item.

But when resizing is required, all existing elements must be copied:

```text
O(n)
```

Therefore its worst case is:

```text
O(n)
```

However, because capacity doubles during resizing, resizing happens relatively rarely.

This leads to the idea of **amortised O(1)** insertion.

---

# 28. Stack vs Queue

| Feature | Stack | Queue |
|---|---|---|
| Rule | LIFO | FIFO |
| Insert | `push` | `enqueue` |
| View next | `peek` | `peek` |
| Remove | `pop` | `dequeue` |
| Accessed position | Top | Front |
| Typical example | Undo | Printer jobs |

---

# 29. Important Hurdle Concepts

Make sure you can explain and implement:

```text
Heap vs Stack
      ↓
new / delete
new[] / delete[]
      ↓
Pointer arithmetic
arr[i] == *(arr + i)
      ↓
Dynamic arrays
      ↓
Dynamic 2D arrays
      ↓
Stack
LIFO
push / peek / pop
      ↓
Queue
FIFO
enqueue / peek / dequeue
      ↓
Circular Queue
rear / size / front
      ↓
Time complexity
```

---

# 中文逐步讲解

## 1. 为什么又开始讲 Heap？

你前面已经学过：

```cpp
Node* new_node = new Node;
```

如果写：

```cpp
void append(int value) {
    Node new_node;
}
```

`new_node` 是 local variable，它在 stack 上。函数结束后，它就被自动释放。

而：

```cpp
Node* new_node = new Node;
```

会在 heap 上创建 `Node`，函数结束时这个 heap object 不会自动消失。

所以之前 Linked List 的：

```cpp
new Node
```

和现在：

```cpp
new int[100]
```

本质上是同一个概念：**dynamic memory allocation**。

---

## 2. `new int[100]` 返回什么？

```cpp
int* arr = new int[100];
```

右边会在 heap 上创建 100 个连续的 `int`：

```text
[int][int][int][int] ... 100 个
 ^
 |
 first element
```

然后返回第一个元素的地址。

所以：

```cpp
arr
```

是一个 pointer，指向第一个 `int`。

---

## 3. 为什么 `arr[5] == *(arr + 5)`？

因为：

```cpp
arr + 5
```

表示从 `arr` 指向的位置向后移动 5 个 `int`。

所以：

```cpp
*(arr + 5)
```

表示：

> 找到向后第 5 个 int，然后取出它的值。

因此：

```cpp
arr[5]
```

就是更加方便的写法。

最重要的关系：

```cpp
arr[i] == *(arr + i)
```

---

## 4. 为什么 `arr[12]` 可能不会报错？

如果：

```cpp
int* arr = new int[10];
```

合法 index 是：

```text
0 - 9
```

但是 C++ 通常不会自动检查 bounds。

所以：

```cpp
arr[12]
```

仍然会尝试访问 `arr` 后面的第 12 个整数位置。

这个位置并不属于你的 array。

这叫：

**undefined behaviour**。

它可能正常运行，也可能 crash 或破坏其他 memory。

---

## 5. 为什么 2D dynamic array 是 `int**`？

先看一行：

```cpp
new int[15]
```

返回：

```cpp
int*
```

如果你需要 10 行，那么你需要保存 10 个 `int*`。

所以 outer array 是：

```cpp
new int*[10]
```

它返回第一个 `int*` 的地址。

因此：

```cpp
int** arr
```

意思就是：

> pointer to a pointer to int

结构：

```text
arr
 |
 v
[int*] ---> [int][int][int]
[int*] ---> [int][int][int]
[int*] ---> [int][int][int]
```

---

## 6. 为什么 delete 需要两层？

因为你进行了很多次 allocation：

```cpp
int** arr = new int*[10];
```

一次 outer allocation。

然后：

```cpp
arr[i] = new int[15];
```

每一行又是一次 allocation。

所以要先删除所有 inner arrays：

```cpp
for (int i = 0; i < 10; i++) {
    delete[] arr[i];
}
```

再删除 outer array：

```cpp
delete[] arr;
```

核心原则：

```text
new      ↔ delete
new[]    ↔ delete[]
```

---

## 7. Stack 数据结构解决什么问题？

Stack 是：

```text
LIFO
Last In, First Out
```

例如 Undo：

```text
Action 1
Action 2
Action 3
```

按 Ctrl+Z 时最先 undo：

```text
Action 3
```

所以只需要访问最后加入的 item。

Stack 的三个主要操作：

```text
push
peek
pop
```

---

## 8. Stack `push()` 的逻辑

假设：

```text
top
 |
 v
[3] -> [2] -> [1]
```

执行：

```cpp
push(4);
```

先创建：

```text
[4]
```

然后：

```cpp
new_node->next = top;
```

得到：

```text
[4] -> [3] -> [2] -> [1]
```

最后：

```cpp
top = new_node;
```

让 `top` 指向 `[4]`。

所以 push 不需要 traversal：

```text
O(1)
```

---

## 9. `pop()` 为什么要先保存 next？

如果直接：

```cpp
delete top;
```

你可能失去下一个 node 的地址。

所以先：

```cpp
Node* new_top = top->next;
```

保存下一个 node。

再：

```cpp
delete top;
top = new_top;
```

核心思想：

> Before deleting or overwriting a pointer, save any address that you still need.

---

## 10. Stack 为什么都是 O(1)？

因为：

```text
push → only top
peek → only top
pop  → only top
```

都不需要遍历整个 linked structure。

因此：

```text
push O(1)
peek O(1)
pop  O(1)
```

---

## 11. Queue 和 Stack 的区别

Stack：

```text
LIFO
```

```text
push A
push B
push C

pop -> C
```

Queue：

```text
FIFO
```

```text
enqueue A
enqueue B
enqueue C

dequeue -> A
```

典型应用：

```text
Stack -> Undo
Queue -> Printer jobs
```

---

## 12. 为什么普通 Queue 会浪费空间？

假设：

```text
[A][B][C][D][E]
```

dequeue 三次以后：

```text
[x][x][x][D][E]
```

前面三个位置已经没有用了。

但如果 `rear` 只不断向右走，就无法重新使用前面的空间。

如果每次 dequeue 都把数据往左移动：

```text
[D][E][ ][ ][ ]
```

就需要移动很多元素：

```text
O(n)
```

所以使用 circular queue。

---

## 13. Circular Queue

核心思想：

> 不移动数据，而是让 index 绕回数组开头。

比如 capacity = 5：

```text
0 1 2 3 4
```

rear 到达最后以后：

```cpp
rear++;
```

如果等于 capacity：

```cpp
if (rear == current_capacity) {
    rear = 0;
}
```

也可以写：

```cpp
rear = (rear + 1) % current_capacity;
```

---

## 14. Circular Queue 的 front 怎么计算？

公式：

```cpp
(rear - size + current_capacity) % current_capacity
```

例如：

```text
capacity = 5
rear = 2
size = 4
```

那么：

```text
front = (2 - 4 + 5) % 5
      = 3
```

如果 array：

```text
[C][D][ ][A][B]
```

logical queue 是：

```text
A -> B -> C -> D
```

所以 front 确实是 index 3。

---

## 15. Queue 的时间复杂度

### `peek`

只计算 front 并访问一次：

```text
O(1)
```

### `dequeue`

也只计算 front、读取 value、修改 size：

```text
O(1)
```

### `enqueue`

通常只是在 `rear` 放一个 item：

```text
O(1)
```

但如果 array 满了，需要 resize 并复制 n 个元素：

```text
O(n)
```

因此：

```text
Best case: O(1)
Worst case: O(n)
```

由于 capacity 每次通常翻倍，因此长期平均下来经常称为：

```text
amortised O(1)
```

---

# Hurdle 复习重点

你应该能够不看答案写出：

```cpp
int* arr = new int[n];
delete[] arr;
```

动态二维数组：

```cpp
int** arr = new int*[n];

for (int i = 0; i < n; i++) {
    arr[i] = new int[n];
}
```

删除：

```cpp
for (int i = 0; i < n; i++) {
    delete[] arr[i];
}

delete[] arr;
```

还应该能够自己实现：

```text
Linked Stack
- push
- peek
- pop
```

以及：

```text
Circular Queue
- enqueue
- peek
- dequeue
- resize
```

最核心的知识连接：

```text
Pointer
   ↓
Heap
   ↓
Dynamic Array
   ↓
Dynamic 2D Array
```

以及：

```text
Linked List
   ↓
Stack
```

以及：

```text
Dynamic Array
   ↓
Queue
   ↓
Circular Queue
```
