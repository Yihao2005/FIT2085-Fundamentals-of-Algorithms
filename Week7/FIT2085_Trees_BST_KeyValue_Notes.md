# FIT2085 — Trees, Binary Trees, BSTs and Key-Value BSTs

## 1. Convertible Problems and Modelling

A useful algorithmic idea is to **convert one problem into another problem that we already know how to solve**.

Example:

- Problem A: generate binary representations of numbers.
- Problem B: generate all subsets of a set.

Suppose we have `m` items. We can use an `m`-bit binary number to represent whether each item is selected:

- `0` = not selected
- `1` = selected

For example, for `{A, B, C}`:

```text
000 -> {}
001 -> {C}
010 -> {B}
011 -> {B, C}
100 -> {A}
...
111 -> {A, B, C}
```

There are:

\[
2^m
\]

possible subsets, so we can generate binary values from `0` to:

\[
2^m - 1
\]

This is an example of **problem reduction**.

### Modelling

A model simplifies a real problem while preserving the structure needed to solve it.

Examples:

```text
Road intersections -> Nodes
Roads              -> Edges

Users              -> Nodes
Friendships        -> Edges

Routers            -> Nodes
Connections        -> Edges
```

---

# 2. Graphs

A **graph** consists of:

- **Nodes / Vertices**: entities
- **Edges**: relationships between entities

Example:

```text
A ----- B
|       |
|       |
C ----- D
```

---

# 3. Trees

A **Tree** is a special graph satisfying:

\[
\boxed{\text{Connected + No Cycles}}
\]

Another equivalent property:

> There is exactly one path between every pair of nodes.

Example:

```text
        A
       / \
      B   C
     / \
    D   E
```

Important terms:

- **Root**: starting/top node
- **Parent**: node above another node
- **Child**: node directly below another node
- **Leaf**: node with no children

The root can be chosen from any node in an undirected tree. Once a root is chosen, parent-child relationships are determined.

---

# 4. Height and Depth

## Height

Tree **height** is the number of edges from the root to the farthest leaf.

Example:

```text
        A
       / \
      B   C
     /
    D
```

Longest path:

```text
A -> B -> D
```

There are 2 edges, so:

\[
\boxed{height = 2}
\]

A single-node tree:

```text
A
```

has:

\[
\boxed{height = 0}
\]

because there are zero edges from the root to itself.

## Depth

The **depth of a node** is the number of edges from the root to that node.

For:

```text
        A
       / \
      B   C
     /
    D
```

```text
depth(A) = 0
depth(B) = 1
depth(C) = 1
depth(D) = 2
```

### Height vs Depth

```text
Depth:
root -> particular node

Height:
root -> farthest leaf
```

---

# 5. Subtrees

Choose any node and include that node plus all of its descendants.

Example:

```text
        A
       / \
      B   C
     / \
    D   E
```

The subtree rooted at `B` is:

```text
      B
     / \
    D   E
```

A subtree is still a tree because:

1. It cannot contain a cycle, otherwise the original tree would contain one.
2. It remains connected because every descendant has a path to the subtree root.

Subtrees are fundamental to recursive tree algorithms.

---

# 6. Balanced Trees

A tree is **balanced** if, for every node, the heights of the subtrees rooted at its children differ by at most 1.

For a Binary Tree:

\[
\boxed{|height(left)-height(right)| \le 1}
\]

This must hold for **every node**, not only the root.

A tree can look visually asymmetric and still be balanced.

---

# 7. Binary Trees

A **Binary Tree** is a tree where each node has at most two children:

- left child
- right child

Possible child counts:

```text
0
1
2
```

but not 3 or more.

Example:

```text
       A
      / \
     B   C
```

A Binary Tree does **not** automatically satisfy any ordering rule.

Example:

```text
       10
      /  \
    500   2
```

This is a valid Binary Tree.

---

# 8. Balanced Binary Tree Height

In a perfect-looking Binary Tree:

```text
depth 0 -> 1 node
depth 1 -> 2 nodes
depth 2 -> 4 nodes
...
depth h -> 2^h nodes
```

Total nodes:

\[
n = 1 + 2 + 4 + \cdots + 2^h
\]

Using the geometric series:

\[
n = 2^{h+1} - 1
\]

Therefore:

\[
n+1 = 2^{h+1}
\]

Taking log base 2:

\[
h = \log_2(n+1)-1
\]

So asymptotically:

\[
\boxed{h = O(\log n)}
\]

This is why balanced binary trees can support efficient operations.

---

# 9. Binary Tree Implementation in C++

A Binary Tree node can be represented as:

```cpp
struct Node {
    int value;
    Node* left;
    Node* right;
};
```

The tree itself only needs a pointer to the root:

```cpp
struct BinaryTree {
    Node* root;
};
```

This is similar to a linked list:

```cpp
struct Node {
    int value;
    Node* next;
};
```

except each tree node can point to two children rather than one next node.

---

# 10. Tree Recursion Pattern

For many Tree algorithms, think in four steps:

1. What is the base case?
2. What is the answer for the left subtree?
3. What is the answer for the right subtree?
4. How do I combine them with the current node?

General shape:

```text
                 current
                 /     \
              left     right
                ↓        ↓
             recurse  recurse
                \        /
                 \      /
                combine
                   ↓
                 answer
```

---

# 11. Sum of All Values

For any node:

\[
sum(T) = current.value + sum(left) + sum(right)
\]

Implementation:

```cpp
int get_sum_helper(Node* current_node) {
    if (current_node == nullptr) {
        return 0;
    }

    return current_node->value
         + get_sum_helper(current_node->left)
         + get_sum_helper(current_node->right);
}
```

The empty subtree returns `0` because it contributes no value.

Time complexity:

\[
\boxed{O(n)}
\]

because every node must be visited.

---

# 12. Tree Height Recursively

For edge-based height:

\[
height(T)=1+\max(height(left),height(right))
\]

To make a leaf return height `0`, define:

\[
\boxed{height(nullptr)=-1}
\]

Implementation:

```cpp
int get_height_helper(Node* current_node) {
    if (current_node == nullptr) {
        return -1;
    }

    int left_height =
        get_height_helper(current_node->left);

    int right_height =
        get_height_helper(current_node->right);

    return 1 + std::max(left_height, right_height);
}
```

A leaf has:

```text
left height  = -1
right height = -1
```

so:

\[
1 + \max(-1,-1)=0
\]

Time complexity:

\[
\boxed{O(n)}
\]

because both left and right subtree heights must be computed.

---

# 13. Checking Whether a Tree Is Balanced

A Binary Tree is balanced if:

\[
|height(left)-height(right)| \le 1
\]

at every node.

A direct implementation:

```cpp
bool is_balanced_helper(Node* current_node) {
    if (current_node == nullptr) {
        return true;
    }

    int left_height =
        get_height_helper(current_node->left);

    int right_height =
        get_height_helper(current_node->right);

    if (std::abs(left_height - right_height) > 1) {
        return false;
    }

    return is_balanced_helper(current_node->left)
        && is_balanced_helper(current_node->right);
}
```

However, this can repeatedly recompute heights.

Worst-case complexity:

\[
\boxed{O(n^2)}
\]

for a highly skewed tree.

---

# 14. Optimised Balanced Check

We can calculate height and balance together.

Convention:

- return normal subtree height if balanced
- return `-2` if unbalanced

```cpp
int check_height(Node* node) {
    if (node == nullptr) {
        return -1;
    }

    int left_height = check_height(node->left);

    if (left_height == -2) {
        return -2;
    }

    int right_height = check_height(node->right);

    if (right_height == -2) {
        return -2;
    }

    if (std::abs(left_height - right_height) > 1) {
        return -2;
    }

    return 1 + std::max(left_height, right_height);
}

bool is_balanced(Node* root) {
    return check_height(root) != -2;
}
```

Now every node is processed only once:

\[
\boxed{O(n)}
\]

---

# 15. Binary Search Trees

A **Binary Search Tree (BST)** is a Binary Tree with an ordering rule.

For every node:

\[
\boxed{
\text{all values in left subtree}
<
\text{current value}
<
\text{all values in right subtree}
}
\]

Example:

```text
          20
         /  \
       10    30
      / \    / \
     5  15  25 40
```

Important:

The rule applies to the **whole subtree**, not just direct children.

This is NOT a BST:

```text
       20
      /  \
    10    30
      \
       25
```

because `25` belongs to the left subtree of `20`, yet:

\[
25 > 20
\]

---

# 16. BST Search

BST Search compares the target with the current node.

```text
equal?
-> found

target smaller?
-> search left

target larger?
-> search right
```

Implementation:

```cpp
Node* search_helper(Node* current_node, int value) {
    if (current_node == nullptr) {
        return nullptr;
    }

    if (current_node->value == value) {
        return current_node;
    }

    if (value < current_node->value) {
        return search_helper(current_node->left, value);
    }

    return search_helper(current_node->right, value);
}
```

Search follows only one path.

General complexity:

\[
\boxed{O(h)}
\]

Balanced BST:

\[
\boxed{O(\log n)}
\]

Worst-case unbalanced BST:

\[
\boxed{O(n)}
\]

Best case, target at root:

\[
\boxed{O(1)}
\]

---

# 17. BST Insert

Insertion is essentially:

> Search until the correct position is `nullptr`, then create a new node there.

Example:

```text
        20
       /  \
     10    30
```

Insert `25`:

```text
25 > 20 -> right
25 < 30 -> left
30->left is nullptr
```

Result:

```text
        20
       /  \
     10    30
           /
          25
```

Recursive implementation:

```cpp
Node* insert_helper(Node* current_node, int value) {
    if (current_node == nullptr) {
        Node* new_node = new Node;
        new_node->value = value;
        new_node->left = nullptr;
        new_node->right = nullptr;

        return new_node;
    }

    if (value == current_node->value) {
        return current_node;
    }

    if (value < current_node->value) {
        current_node->left =
            insert_helper(current_node->left, value);
    } else {
        current_node->right =
            insert_helper(current_node->right, value);
    }

    return current_node;
}
```

Public interface:

```cpp
void insert(int value) {
    root = insert_helper(root, value);
}
```

Important idea:

\[
\boxed{\text{Recursive helper returns the new root of the subtree}}
\]

---

# 18. Ordinary BST Insert Does Not Guarantee Balance

A normal BST only guarantees ordering.

It does **not** automatically rebalance itself.

Insert:

```text
10
20
30
40
50
```

produces:

```text
10
 \
  20
    \
     30
       \
        40
          \
           50
```

This is a valid BST, but not balanced.

Therefore:

\[
h=O(n)
\]

and Search / Insert / Delete can degrade to:

\[
\boxed{O(n)}
\]

Self-balancing BSTs such as:

- AVL Trees
- Red-Black Trees

perform rotations/rebalancing to keep height under control.

---

# 19. BST Delete

Deletion must preserve the BST structure.

There are three cases.

## Case 1 — 0 Children

Delete a leaf directly.

```text
        20
       /  \
     10    30
          /
         25
```

Delete `25`:

```text
        20
       /  \
     10    30
```

## Case 2 — 1 Child

Connect the parent directly to the child.

Before:

```text
20
 \
  30
    \
     40
```

Delete `30`:

```text
20
 \
  40
```

## Case 3 — 2 Children

Use the **successor**.

---

# 20. Successor

The successor of a node is:

> the smallest value greater than that node.

In sorted order:

```text
10 20 25 30 40
```

The successor of `20` is:

```text
25
```

To find the successor when the node has a right subtree:

\[
\boxed{\text{Right once, then left as far as possible}}
\]

Example:

```text
             50
           /    \
         30      80
                /  \
              60    90
             /
           55
```

Successor of `50`:

```text
50 -> right -> 80
80 -> left  -> 60
60 -> left  -> 55
```

Therefore:

\[
\boxed{successor(50)=55}
\]

---

# 21. Why the Successor Has No Left Child

Suppose `W` is the successor, but it has a left child `X`.

Because `X` is in the right subtree of the original node `V`:

\[
X > V
\]

Because `X` is the left child of `W`:

\[
X < W
\]

Therefore:

\[
V < X < W
\]

This contradicts the claim that `W` is the immediate next larger value.

Therefore a successor can have:

- 0 children
- 1 right child

but no left child.

This makes successor deletion reduce to Case 1 or Case 2.

---

# 22. Recursive BST Delete

A clean implementation returns the new root of the modified subtree.

```cpp
Node* delete_helper(Node* current_node, int value) {
    if (current_node == nullptr) {
        return nullptr;
    }

    if (value < current_node->value) {
        current_node->left =
            delete_helper(current_node->left, value);

        return current_node;
    }

    if (value > current_node->value) {
        current_node->right =
            delete_helper(current_node->right, value);

        return current_node;
    }

    // Found target

    // 0 children
    if (
        current_node->left == nullptr &&
        current_node->right == nullptr
    ) {
        delete current_node;
        return nullptr;
    }

    // only right child
    if (current_node->left == nullptr) {
        Node* replacement = current_node->right;
        delete current_node;
        return replacement;
    }

    // only left child
    if (current_node->right == nullptr) {
        Node* replacement = current_node->left;
        delete current_node;
        return replacement;
    }

    // 2 children
    Node* successor = find_min(current_node->right);

    current_node->value = successor->value;

    current_node->right =
        delete_helper(
            current_node->right,
            successor->value
        );

    return current_node;
}
```

Public function:

```cpp
void delete_value(int value) {
    root = delete_helper(root, value);
}
```

---

# 23. `find_min`

In a BST, smaller values are always to the left.

So the minimum value in a subtree is found by going left until impossible.

```cpp
Node* find_min(Node* current_node) {
    while (current_node->left != nullptr) {
        current_node = current_node->left;
    }

    return current_node;
}
```

---

# 24. BST Operation Complexity

For Search, Insert and Delete:

\[
\boxed{O(h)}
\]

because they follow paths whose lengths are bounded by tree height.

| Operation | Balanced BST | Worst Unbalanced BST |
|---|---:|---:|
| Search | \(O(\log n)\) | \(O(n)\) |
| Insert | \(O(\log n)\) | \(O(n)\) |
| Delete | \(O(\log n)\) | \(O(n)\) |

---

# 25. Key-Value BST

A real BST often needs to associate searchable keys with actual data.

Example:

```text
"15-inch laptop" -> 1799.99
```

Here:

```text
key   = "15-inch laptop"
value = 1799.99
```

The key controls:

- ordering
- searching
- insertion position
- deletion position

The value is attached data.

Core idea:

\[
\boxed{\text{BST uses the key to organise/search; value is attached data}}
\]

---

# 26. Key-Value Node

Example using product name and price:

```cpp
struct Node {
    std::string key;
    double value;

    Node* left = nullptr;
    Node* right = nullptr;
};
```

The BST compares only:

```cpp
key
```

not:

```cpp
value
```

---

# 27. Key-Value Insert

```cpp
Node* insert_helper(
    Node* current_node,
    std::string key,
    double value
) {
    if (current_node == nullptr) {
        Node* new_node = new Node;

        new_node->key = key;
        new_node->value = value;

        return new_node;
    }

    if (key < current_node->key) {
        current_node->left =
            insert_helper(
                current_node->left,
                key,
                value
            );
    }
    else if (key > current_node->key) {
        current_node->right =
            insert_helper(
                current_node->right,
                key,
                value
            );
    }
    else {
        current_node->value = value;
    }

    return current_node;
}
```

When the key already exists, one reasonable map-like design is:

```text
same key
-> update associated value
```

---

# 28. Key-Value Search

```cpp
Node* search_helper(
    Node* current_node,
    std::string key
) {
    if (current_node == nullptr) {
        return nullptr;
    }

    if (key == current_node->key) {
        return current_node;
    }

    if (key < current_node->key) {
        return search_helper(
            current_node->left,
            key
        );
    }

    return search_helper(
        current_node->right,
        key
    );
}
```

Example:

```cpp
Node* result = tree.search("Laptop");

if (result != nullptr) {
    std::cout << result->value;
}
```

The BST can efficiently search by the chosen key, but not automatically by some other field.

If the tree is ordered by product name, it cannot efficiently search by price unless another index/data structure is built.

---

# 29. Key-Value Delete

Deletion still uses the key for comparisons.

For a two-child node, both key and value must be copied from the successor:

```cpp
current_node->key =
    successor->key;

current_node->value =
    successor->value;
```

They form one logical pair:

\[
\boxed{(key,value)}
\]

Copying only the key would mismatch the data.

---

# 30. Key-Value Structures and Maps

Key-value BSTs lead naturally to map/dictionary abstractions.

Examples:

```text
Student ID -> Student object
Username   -> User account
Product ID -> Product
ISBN       -> Book
Word       -> Definition
```

Conceptually:

```cpp
map<string, double>
```

is also:

```text
key -> value
```

C++ `std::map` is typically implemented using a balanced tree such as a Red-Black Tree.

---

# 31. Tree Traversal

Sometimes we need to visit every node.

This is called **Tree Traversal**.

Common DFS traversal orders:

```text
Pre-order
In-order
Post-order
```

---

# 32. Pre-order Traversal

Order:

```text
Node
Left
Right
```

Mnemonic:

```text
NLR
```

Implementation:

```cpp
void preorder(Node* node) {
    if (node == nullptr) {
        return;
    }

    std::cout << node->key << '\n';

    preorder(node->left);
    preorder(node->right);
}
```

---

# 33. In-order Traversal

Order:

```text
Left
Node
Right
```

Mnemonic:

```text
LNR
```

Implementation:

```cpp
void inorder(Node* node) {
    if (node == nullptr) {
        return;
    }

    inorder(node->left);

    std::cout << node->key << '\n';

    inorder(node->right);
}
```

For a BST, in-order traversal outputs keys in sorted order.

Example:

```text
        20
       /  \
     10    30
    / \    / \
   5  15  25 40
```

In-order:

```text
5 10 15 20 25 30 40
```

because:

\[
left < node < right
\]

---

# 34. Post-order Traversal

Order:

```text
Left
Right
Node
```

Mnemonic:

```text
LRN
```

Implementation:

```cpp
void postorder(Node* node) {
    if (node == nullptr) {
        return;
    }

    postorder(node->left);
    postorder(node->right);

    std::cout << node->key << '\n';
}
```

Post-order is useful when deleting a whole tree because children must be processed before their parent.

---

# 35. Memory Management

Tree nodes created with:

```cpp
new Node
```

live on the heap.

Deleting only the root:

```cpp
delete root;
```

does not free the descendants.

That would create a memory leak.

Correct recursive destruction:

```cpp
void destroy(Node* current_node) {
    if (current_node == nullptr) {
        return;
    }

    destroy(current_node->left);
    destroy(current_node->right);

    delete current_node;
}
```

Destructor:

```cpp
~BinarySearchTree() {
    destroy(root);
}
```

This is a post-order process:

```text
Left
Right
Node
```

---

# 36. Why Tree Algorithms Have Different Time Complexities

The important question is not:

> Is the function recursive?

Instead ask:

> How many recursive branches/nodes does it actually process?

---

# 37. BST Search Recurrence

A balanced BST Search follows only one side:

\[
T(n)=T(n/2)+O(1)
\]

Therefore:

\[
\boxed{O(\log n)}
\]

For a skewed tree:

\[
T(n)=T(n-1)+O(1)
\]

Therefore:

\[
\boxed{O(n)}
\]

---

# 38. Sum Recurrence

Sum explores both sides:

\[
T(n)=2T(n/2)+O(1)
\]

Every node is processed once:

\[
\boxed{O(n)}
\]

---

# 39. Height Complexity

Although the final expression uses:

```cpp
max(height(left), height(right))
```

both recursive calls must still execute before `max` can choose the larger result.

Therefore Height visits every node:

\[
\boxed{O(n)}
\]

---

# 40. Traversal Complexity

Pre-order, In-order and Post-order visit every node exactly once.

Therefore:

\[
\boxed{O(n)}
\]

even for a balanced tree.

Balanced height being \(O(\log n)\) does **not** make every Tree algorithm \(O(\log n)\).

---

# 41. Recursive Space Complexity

Recursive Tree algorithms use the call stack.

Maximum recursion depth is proportional to tree height:

\[
\boxed{O(h)}
\]

Balanced tree:

\[
\boxed{O(\log n)}
\]

Worst-case skewed tree:

\[
\boxed{O(n)}
\]

This is auxiliary **space complexity**, not necessarily time complexity.

---

# 42. Final Complexity Summary

| Algorithm / Operation | Time Complexity |
|---|---:|
| BST Search | \(O(h)\) |
| BST Insert | \(O(h)\) |
| BST Delete | \(O(h)\) |
| Sum all nodes | \(O(n)\) |
| Tree Height | \(O(n)\) |
| Pre-order Traversal | \(O(n)\) |
| In-order Traversal | \(O(n)\) |
| Post-order Traversal | \(O(n)\) |
| Naive Balanced Check | worst \(O(n^2)\) |
| Optimised Balanced Check | \(O(n)\) |

For BST height:

\[
h=
\begin{cases}
O(\log n), & \text{balanced}\\
O(n), & \text{worst unbalanced}
\end{cases}
\]

---

# 43. Five Core Ideas to Remember

## 1. Tree definition

\[
\boxed{\text{Tree = Connected + No Cycles}}
\]

## 2. Height

Height counts edges from the root to the farthest leaf.

Balanced Binary Tree:

\[
\boxed{h=O(\log n)}
\]

## 3. Tree recursion

Think:

```text
solve left subtree
solve right subtree
combine with current node
```

## 4. BST rule

Not just direct children:

\[
\boxed{
ALL(left) < node < ALL(right)
}
\]

## 5. Delete with two children

Use the successor:

\[
\boxed{\text{Right once, then Left as far as possible}}
\]

---

# 44. Big Picture

```text
Convertible Problems / Modelling
        ↓
Graph
Nodes + Edges
        ↓
Tree
Connected + No Cycles
        ↓
Root / Parent / Child / Leaf
        ↓
Height / Depth / Subtree
        ↓
Balanced Tree
        ↓
Binary Tree
max 2 children
        ↓
C++ pointers
value + left* + right*
        ↓
Tree Recursion
sum / height / balanced
        ↓
Binary Search Tree
left < node < right
        ↓
Search / Insert / Delete
        ↓
Successor
right once + left as far as possible
        ↓
Balanced vs Unbalanced
O(log n) vs O(n)
        ↓
Key-Value BST
key controls ordering
value is attached data
        ↓
Tree Traversal
Pre / In / Post
        ↓
Memory Management
Post-order destruction
```
