# FIT1008 / FIT1054 / FIT2085 — Heaps, Heapify, Heap Sort, and Priority Queues

## 1. Motivation: The Orders Book Problem

A stock market maintains an **orders book** containing buy and sell orders.

For matching trades, the system mainly needs to know:

- the **highest buying price**, and
- the **lowest selling price**.

A trade can occur when the highest buy offer is high enough to match the lowest sell offer.

This creates a data-structure problem. We want a structure that can efficiently:

1. add a new value,
2. access the current maximum value, and
3. remove the current maximum value.

If we maintain all values in a sorted array or linked list, inserting a new value in the correct position can require shifting or traversing many elements.

Worst-case insertion:

\[
O(n)
\]

For a system containing millions of orders, this is undesirable.

A **heap** is designed to support the important operations much more efficiently.

---

## 2. What Is a Heap?

A heap is a binary-tree-based data structure.

A **Max Heap** maintains two important invariants.

### 2.1 Heap Order

Every parent is greater than or equal to its children:

\[
parent \geq children
\]

Example:

```text
        10
       /  \
      8    7
     / \
    3   5
```

Because this rule holds recursively throughout the tree, the root must contain the maximum value.

Therefore:

\[
\boxed{\text{maximum value = root}}
\]

However, a heap is **not fully sorted**.

For example:

```text
        10
       /  \
      2    9
```

is a valid Max Heap.

There is no requirement that the left child must be smaller than the right child.

---

### 2.2 Heap Completeness

A heap must also be a **complete binary tree**.

This means:

- levels are filled from top to bottom;
- nodes within a level are filled from left to right;
- there are no unnecessary holes before the last node.

Valid:

```text
        10
       /  \
      8    7
     / \  /
    4  5 3
```

Invalid:

```text
        10
       /  \
      8    7
          /
         3
```

Completeness is important because a complete binary tree with \(n\) nodes has height:

\[
O(\log n)
\]

This allows heap operations that move along one root-to-leaf or leaf-to-root path to run in \(O(\log n)\).

---

## 3. Max Heap vs Min Heap

### Max Heap

A Max Heap maintains:

\[
parent \geq children
\]

Therefore:

\[
root = maximum
\]

Typical applications include:

- highest-priority task;
- highest buying price;
- repeatedly retrieving the largest value.

### Min Heap

A Min Heap maintains:

\[
parent \leq children
\]

Therefore:

\[
root = minimum
\]

Typical applications include:

- lowest selling price;
- smallest distance;
- earliest deadline;
- repeatedly retrieving the smallest value.

For an orders book, a possible design is:

```text
Buy orders  -> Max Heap -> highest buying price
Sell orders -> Min Heap -> lowest selling price
```

---

# 4. Inserting into a Max Heap

When inserting a new value, the first priority is to preserve **heap completeness**.

Therefore, the new node is always inserted into the next available position at the end of the complete tree.

Suppose the heap is:

```text
        10
       /  \
      8    7
     / \
    3   5
```

Insert `12`:

```text
        10
       /  \
      8    7
     / \  /
    3  5 12
```

The structure is complete, but Heap Order is violated because:

\[
12 > 7
\]

Swap `12` with its parent:

```text
        10
       /  \
      8    12
     / \  /
    3  5 7
```

The violation still exists:

\[
12 > 10
\]

Swap again:

```text
        12
       /  \
      8    10
     / \  /
    3  5 7
```

The heap is now valid.

This process is called:

- **bubble up**,
- **sift up**, or
- **rise**.

---

## 5. Why Bubble Up Works

Before insertion, assume the heap is valid.

If a newly inserted value is larger than its parent, swapping the two fixes the violation between them.

Suppose:

```text
        7
       / \
      5  12
```

Because the heap was valid before `12` was inserted, the old parent `7` was already at least as large as its other child `5`.

Since:

\[
12 > 7 \geq 5
\]

placing `12` in the parent's position cannot violate Heap Order with the other child.

Therefore, only the path from the new node toward the root needs to be checked.

---

## 6. Insertion Complexity

In the best case, the inserted value is already less than or equal to its parent.

No swaps are required:

\[
\boxed{O(1)}
\]

In the worst case, the inserted value is larger than every ancestor and bubbles from the bottom to the root.

A complete binary tree has height:

\[
O(\log n)
\]

Therefore:

\[
\boxed{\text{Insert worst case} = O(\log n)}
\]

---

# 7. Extracting the Maximum

In a Max Heap, the maximum value is always at the root.

Simply reading the maximum is therefore:

\[
\boxed{O(1)}
\]

However, **extracting** it is more complicated because removing the root directly would break heap completeness.

Suppose:

```text
        12
       /  \
      8    10
     / \   /
    3   5 7
```

The maximum is `12`.

### Step 1: Save the maximum

Store `12` so it can be returned later.

### Step 2: Swap the root and the last node

```text
        7
       / \
      8  10
     / \  /
    3  5 12
```

### Step 3: Remove the last node

```text
        7
       / \
      8  10
     / \
    3   5
```

Completeness has been preserved, but Heap Order may now be broken.

### Step 4: Sink the new root

The node `7` has children `8` and `10`.

For a Max Heap, it must swap with the **larger child**, so it swaps with `10`:

```text
        10
       /  \
      8    7
     / \
    3   5
```

The heap is valid again.

This process is called:

- **sink**,
- **sift down**, or
- **bubble down**.

---

## 8. Why Sink Uses the Larger Child

Suppose:

```text
       4
      / \
     8  10
```

If `4` swaps with `8`:

```text
       8
      / \
     4  10
```

Heap Order is still violated because:

\[
8 < 10
\]

Instead, swap with the larger child:

```text
       10
      /  \
     8    4
```

Now the new parent is greater than or equal to both children.

Therefore, during a Max Heap sink operation:

\[
\boxed{\text{swap with the larger child}}
\]

For a Min Heap, the equivalent operation swaps with the smaller child.

---

## 9. Extract-Max Complexity

In the worst case, the replacement root may need to sink all the way to the bottom.

The distance is at most the height of the tree:

\[
O(\log n)
\]

Therefore:

\[
\boxed{\text{extract\_max} = O(\log n)}
\]

A useful distinction is:

\[
\boxed{\text{peek max} = O(1)}
\]

but

\[
\boxed{\text{extract max} = O(\log n)}
\]

---

# 10. Searching in a Heap

A heap is efficient for retrieving an extreme value, but it is not designed for arbitrary search.

Consider:

```text
        20
       /  \
      10   18
     / \   / \
    7   8 3  15
```

Suppose we search for `8`.

From `20`, knowing that:

\[
8 < 20
\]

does not tell us whether to go left or right.

Unlike a Binary Search Tree, there is no ordering relationship between the two subtrees.

Therefore, an arbitrary search may need to inspect every node:

\[
\boxed{O(n)}
\]

This illustrates an important data-structure trade-off:

> A heap sacrifices efficient arbitrary searching in order to provide efficient access to the maximum or minimum.

---

# 11. Array-Based Heap Representation

Heaps are normally represented using arrays rather than linked tree nodes.

This works particularly well because a heap is complete.

Number the nodes from top to bottom and left to right:

```text
        1
      /   \
     2     3
    / \   / \
   4   5 6   7
```

Using **1-based indexing**, for a node at index \(k\):

\[
\boxed{parent(k)=\left\lfloor\frac{k}{2}\right\rfloor}
\]

\[
\boxed{left(k)=2k}
\]

\[
\boxed{right(k)=2k+1}
\]

Example for node 3:

```text
parent = 3 / 2 = 1
left   = 6
right  = 7
```

This means explicit pointers are unnecessary.

---

## 12. Tree and Array Represent the Same Heap

Tree:

```text
        50
       /  \
      20   28
     / \   /
    1   2 0
```

Array:

```text
Index:  1   2   3   4   5   6
Value: 50  20  28   1   2   0
```

The array is simply the complete tree written level by level.

---

## 13. Why Index 0 Is Often Unused

With 1-based indexing:

```cpp
parent(i) = i / 2;
left(i) = 2 * i;
right(i) = 2 * i + 1;
```

The formulas are simple.

A heap can also use 0-based indexing, in which case:

```cpp
parent(i) = (i - 1) / 2;
left(i) = 2 * i + 1;
right(i) = 2 * i + 2;
```

The course implementation uses 1-based indexing for clarity.

---

# 14. Dynamic Array Capacity

If the number of future heap elements is unknown, the underlying array may need to grow.

Instead of increasing capacity by one every time it becomes full, a common strategy is to approximately double the capacity.

Conceptually:

```text
capacity 1
   ↓
capacity 2
   ↓
capacity 4
   ↓
capacity 8
   ↓
capacity 16
```

This avoids reallocating and copying the entire array on every insertion.

---

# 15. Understanding `add()`

A typical insertion first places the value at the end:

```cpp
array[size + 1] = value;
size++;
```

Because the array stores nodes level by level, this automatically preserves completeness.

Then:

```cpp
int current_index = size;
```

tracks the newly inserted value.

Bubble up while the parent exists and is smaller:

```cpp
while (
    parent(current_index) >= 1 &&
    array[parent(current_index)] < array[current_index]
) {
    swap(array[parent(current_index)], array[current_index]);
    current_index = parent(current_index);
}
```

When the loop terminates, either:

- the new value reached the root, or
- its parent is greater than or equal to it.

Therefore Heap Order has been restored.

---

# 16. Understanding `extract_max()`

First save the root:

```cpp
int max_val = array[1];
```

Then swap the root with the last heap element:

```cpp
swap(array[1], array[size]);
```

Reduce the logical heap size:

```cpp
size--;
```

The old maximum still physically exists somewhere in the allocated array, but it is outside the logical heap.

For example:

```text
Physical array:
[5, 20, 28, 1, 2, 50]

Logical heap size = 5
```

Only indices `1...5` are considered part of the heap.

The new root is then sunk until Heap Order is restored.

---

# 17. Understanding `sink()`

A typical loop begins with:

```cpp
while (left_child(node_index) <= size)
```

Why is checking the left child enough?

Because the tree is complete.

A node cannot have a right child without having a left child.

Therefore:

```text
no left child
    ↓
no children
```

The algorithm initially assumes the left child is larger:

```cpp
int larger_child_index = left_child(node_index);
```

If a right child exists and is larger, it selects the right child instead.

Then:

- if the larger child is greater than the current node, swap;
- otherwise stop.

---

# 18. Why Heaps Work Well in Arrays but BSTs Do Not

A heap is complete, so its nodes occupy consecutive array positions.

A Binary Search Tree is not necessarily complete.

Consider a highly unbalanced BST:

```text
1
 \
  2
   \
    3
     \
      4
```

If we tried to use the same array mapping:

```text
1 -> index 1
2 -> index 3
3 -> index 7
4 -> index 15
5 -> index 31
```

The array would contain many unused positions.

Therefore:

- **Heap:** complete structure -> array representation is compact.
- **BST:** potentially sparse/unbalanced -> pointer-based representation is usually more suitable.

---

# 19. Heap Sort

A heap can be used for sorting.

The basic idea is:

```text
Input array
    ↓
Build a Max Heap
    ↓
Repeatedly extract the maximum
    ↓
Sorted values
```

For example:

```text
Input:
4 1 7 3 8 5
```

After building a Max Heap, the exact arrangement may vary, but the root is guaranteed to be `8`.

Repeated extraction gives:

```text
8
7
5
4
3
1
```

Therefore, repeatedly extracting from a separate Max Heap produces values in descending order.

---

## 20. Basic Heap Sort Complexity

If the heap is built by inserting all \(n\) elements individually:

\[
n \times O(\log n)
=
O(n\log n)
\]

Then extracting all \(n\) elements costs:

\[
n \times O(\log n)
=
O(n\log n)
\]

Therefore:

\[
\boxed{\text{Heap Sort} = O(n\log n)}
\]

---

# 21. In-Place Heap Sort

A standard Heap Sort implementation does not need to create a separate output array.

After building a Max Heap, repeatedly:

1. swap the root with the final element of the current heap;
2. reduce the heap size;
3. sink the new root.

Example:

```text
[8, 4, 7, 3, 1, 5]
 ^              ^
root           end
```

Swap:

```text
[5, 4, 7, 3, 1, 8]
                ^
             sorted
```

Now only the first five elements are treated as the heap.

The process repeats.

Because the largest elements are moved to the end one by one, a Max Heap produces an **ascending** array when Heap Sort is performed in place:

```text
[1, 3, 4, 5, 7, 8]
```

The extra space required by the standard iterative array implementation is:

\[
\boxed{O(1)}
\]

This is an important advantage over standard array-based Merge Sort, which normally requires \(O(n)\) auxiliary space.

---

# 22. Heapify

Suppose all \(n\) input values are known in advance.

One approach is:

```text
insert item 1
insert item 2
insert item 3
...
insert item n
```

This can cost:

\[
O(n\log n)
\]

However, knowing all elements in advance allows a more efficient approach called:

- **Heapify**, or
- **Bottom-Up Heap Construction**.

Heapify builds the heap in:

\[
\boxed{O(n)}
\]

---

# 23. Heapify Procedure

First copy all values directly into the array.

This immediately gives the correct complete-tree structure, although Heap Order may be violated.

For example:

```text
Input array
    ↓
Treat it directly as a complete binary tree
```

Then process nodes from the bottom upward.

For each non-leaf node:

```text
sink(node)
```

Eventually the root is processed, and the entire tree becomes a valid heap.

A simple implementation could be:

```cpp
for (int i = n; i >= 1; i--) {
    sink(i);
}
```

However, this performs unnecessary calls on leaf nodes.

---

# 24. Where Should Heapify Start?

In a 1-based heap containing \(n\) elements, every node with index:

\[
i > \left\lfloor\frac n2\right\rfloor
\]

is a leaf.

Leaves have no children, so they cannot violate Heap Order below themselves and do not need to sink.

Therefore the standard implementation starts at:

\[
\left\lfloor\frac n2\right\rfloor
\]

and moves backward:

```cpp
for (int i = n / 2; i >= 1; i--) {
    sink(i);
}
```

This starts at the last non-leaf node.

---

# 25. Why Heapify Is \(O(n)\), Not \(O(n\log n)\)

At first, it may appear that:

```text
n nodes
×
O(log n) sink
=
O(n log n)
```

But this is too pessimistic because most nodes cannot sink \(O(\log n)\) levels.

A complete binary tree is **bottom-heavy**.

Approximately:

```text
n/2 nodes   are leaves        -> 0 levels of sinking
n/4 nodes   are one level up  -> at most 1 level
n/8 nodes   are two levels up -> at most 2 levels
n/16 nodes  are three up      -> at most 3 levels
...
1 root                          -> at most log n levels
```

Therefore the total work is bounded by:

\[
\frac n2(0)
+
\frac n4(1)
+
\frac n8(2)
+
\frac n{16}(3)
+\cdots
\]

This series grows linearly with \(n\).

Therefore:

\[
\boxed{\text{Heapify} = O(n)}
\]

### Key intuition

Most nodes are near the bottom of the tree, where very little work is required.

The nodes that can sink a long distance are near the top, but there are very few of them.

That is why the total is \(O(n)\), even though an individual sink operation can be \(O(\log n)\).

---

# 26. Why Heapify Produces a Valid Heap

Heapify can be justified using a bottom-up correctness argument.

Before processing a node \(i\), its children occur later in the array and have already been processed.

Therefore, the left and right subtrees rooted at the children of \(i\) are already valid heaps.

The only remaining possible violation is between node \(i\) and its children.

Calling:

```cpp
sink(i);
```

moves the value at \(i\) downward until Heap Order is restored.

After `sink(i)`, the entire subtree rooted at \(i\) is therefore a valid heap.

Processing nodes from:

\[
\left\lfloor\frac n2\right\rfloor
\]

down to:

\[
1
\]

repeats this argument upward through the tree.

When node `1` has been processed, the subtree rooted at `1` is the entire tree.

Therefore, the complete tree is a valid heap.

---

# 27. Heapify and Heap Sort Together

The optimized Heap Sort process is:

```text
Input array
    ↓
Heapify
O(n)
    ↓
Max Heap
    ↓
Repeatedly extract/swap maximum
n × O(log n)
    ↓
Sorted array
```

Therefore:

\[
O(n)+O(n\log n)
\]

and the dominant term is:

\[
\boxed{O(n\log n)}
\]

A very important distinction is:

> **Heapify builds a heap; it does not finish the sorting process.**

After Heapify, only Heap Order is guaranteed. The whole array is not globally sorted.

---

# 28. Priority Queue

A normal queue follows:

\[
\text{FIFO}
\]

or:

> First In, First Out.

A Priority Queue behaves differently.

Instead of removing the oldest item, it removes the item with the highest priority.

Example:

```text
Task A -> priority 2
Task B -> priority 10
Task C -> priority 5
```

Removal order:

```text
Task B
Task C
Task A
```

A Max Heap is a natural implementation of a Priority Queue because it provides:

```text
insert                -> O(log n)
peek highest priority -> O(1)
remove highest        -> O(log n)
```

Strictly speaking:

- **Priority Queue** is an abstract data type describing the required behaviour.
- **Heap** is a data structure commonly used to implement that behaviour.

---

# 29. Key-Value Pairs in Heaps

Real heaps usually store objects rather than simple integers.

For example:

```cpp
struct BuyOrder {
    string username;
    int order_timestamp;
    int order_quantity;
    int unit_price;
    string sec_code;
};
```

The heap stores the entire `BuyOrder`, but comparisons may use only:

```cpp
unit_price
```

For example:

```text
Order A:
username = Alice
unit_price = 100

Order B:
username = Bob
unit_price = 150
```

The heap considers Order B to have a larger key because:

\[
150 > 100
\]

When `extract_max()` is called, it returns the entire Order B object, not just the number `150`.

Conceptually:

```text
key   = unit_price
value = complete BuyOrder object
```

---

# 30. Priority Queue Application: Operating-System Scheduling

A computer may have many runnable processes but only a limited number of CPU cores.

Some tasks are more urgent than others.

For example:

```text
Background task      priority 1
Browser rendering    priority 5
Network processing   priority 8
Mouse input          priority 10
```

A priority-based scheduler can select the highest-priority runnable task first.

A Priority Queue can support frequent insertion and removal efficiently.

Real operating-system schedulers are considerably more complicated and may also consider:

- fairness;
- starvation;
- time slices;
- dynamic priorities;
- real-time constraints;
- CPU affinity;
- I/O behaviour.

The Priority Queue example demonstrates the underlying data-structure idea rather than a complete real-world scheduler.

---

# 31. Heap vs BST

| Property | Heap | BST |
|---|---|---|
| Main purpose | Quickly access min/max | Efficient ordered searching |
| Structural requirement | Complete binary tree | Not necessarily complete |
| Ordering rule | Parent vs children | Left subtree vs parent vs right subtree |
| Maximum in Max Heap | Root | Usually right-most node |
| Arbitrary search | \(O(n)\) worst case | \(O(\log n)\) if balanced |
| Insert | \(O(\log n)\) | \(O(\log n)\) if balanced |
| Natural array representation | Yes | Usually no |
| Fully sorted structure | No | Maintains search ordering |

The most important conceptual difference is:

> A heap maintains only enough ordering to make an extreme value easy to access. A BST maintains stronger ordering to support searching.

---

# 32. Complexity Summary

| Operation | Max Heap Complexity |
|---|---:|
| Peek maximum | \(O(1)\) |
| Insert — best case | \(O(1)\) |
| Insert — worst case | \(O(\log n)\) |
| Extract maximum | \(O(\log n)\) |
| Arbitrary search | \(O(n)\) |
| Build heap using repeated insertion | \(O(n\log n)\) |
| Build heap using Heapify | \(O(n)\) |
| Heap Sort | \(O(n\log n)\) |
| Standard in-place Heap Sort auxiliary space | \(O(1)\) |

---

# 33. Core Ideas to Remember

### Heap invariants

A Max Heap requires:

```text
1. Heap Order:
   parent >= children

2. Heap Completeness:
   complete binary tree
```

### Insert

```text
Add at the end
    ↓
Bubble up
    ↓
O(log n) worst case
```

### Extract Max

```text
Save root
    ↓
Swap root with last
    ↓
Remove last from logical heap
    ↓
Sink root using larger child
    ↓
O(log n)
```

### Array representation

For 1-based indexing:

\[
parent(i)=i/2
\]

\[
left(i)=2i
\]

\[
right(i)=2i+1
\]

### Heapify

```text
All values known in advance
    ↓
Copy them into complete-tree positions
    ↓
Start at the last non-leaf node
    ↓
Sink nodes bottom-up
    ↓
Valid heap
```

Complexity:

\[
\boxed{O(n)}
\]

### Heap Sort

```text
Heapify
    ↓
Repeated extract-max / root-last swap
    ↓
Sorted array
```

Complexity:

\[
\boxed{O(n\log n)}
\]

### Priority Queue

```text
Normal Queue:
oldest item first

Priority Queue:
highest-priority item first
```

A heap is a standard efficient implementation of a Priority Queue.

---

# 34. One-Sentence Summary

> **A heap is a complete binary tree that maintains only enough ordering to keep the maximum or minimum at the root, allowing efficient \(O(\log n)\) insertion and extraction, \(O(n)\) bottom-up construction through Heapify, and \(O(n\log n)\) Heap Sort.**
