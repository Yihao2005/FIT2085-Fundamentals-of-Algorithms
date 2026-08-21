# FIT2085 Recursion, Merge Sort and Quick Sort Notes

## 1. Recursion

### 1.1 What is Recursion?

A recursive function is a function that solves a problem by calling itself on a **smaller version of the same problem**.

A recursive solution usually has two essential parts:

1. **Base Case**
   - A small enough input that can be solved directly.
   - It stops the recursion.

2. **Recursive Step**
   - Reduce the current problem into one or more smaller problems of the same type.
   - Use the result of those smaller problems to solve the current problem.

A recursive solution works when:

> If I can solve a problem using the answer to a smaller version of the same problem, and there is a small enough case that I can solve directly, then the problem can be solved recursively.

---

## 2. Example: Find the Largest Item Recursively

Suppose we want to find the largest number in:

```text
[5, 2, 8, 1, 9, 4]
```

Instead of checking every element with a loop, we can think:

```text
Largest of whole array
        ↓
compare
largest of left half
and
largest of right half
```

For example:

```text
[5, 2, 8]      [1, 9, 4]
     ↓              ↓
largest = 8     largest = 9

       8 vs 9
          ↓
          9
```

Each half can be solved using the same idea again.

### Base Case

If the range contains only one item:

```text
[8]
```

then the largest value is simply:

```text
8
```

### C++ Implementation

```cpp
int largest_of_array(int arr[], int start, int end) {
    if (end == start + 1) {
        return arr[start];
    }

    int middle = (start + end) / 2;

    int largest_of_left =
        largest_of_array(arr, start, middle);

    int largest_of_right =
        largest_of_array(arr, middle, end);

    if (largest_of_left > largest_of_right) {
        return largest_of_left;
    } else {
        return largest_of_right;
    }
}
```

The interval is:

```text
[start, end)
```

which means:

- `start` is included.
- `end` is excluded.

For an array with 10 elements:

```cpp
largest_of_array(arr, 0, 10);
```

---

# 3. Recursive Power Function

We want:

```text
n^p
```

## 3.1 Variation 1

Use:

```text
n^p = n × n^(p-1)
```

Base case:

```text
n^1 = n
```

Implementation:

```cpp
int power(int n, int p) {
    if (p == 1) return n;

    return n * power(n, p - 1);
}
```

Example:

```text
power(2, 4)

= 2 × power(2, 3)
= 2 × 2 × power(2, 2)
= 2 × 2 × 2 × power(2, 1)
= 2 × 2 × 2 × 2
= 16
```

---

## 3.2 Variation 2: Faster Power

If `p` is even:

```text
n^p = n^(p/2) × n^(p/2)
```

For example:

```text
2^10 = 2^5 × 2^5
```

If `p` is odd:

```text
n^p = n^(p-1) × n
```

Since `p - 1` is even, we can halve it.

Implementation:

```cpp
int power(int n, int p) {
    if (p == 1) return n;

    if (p % 2 == 0) {
        int half_power = power(n, p / 2);
        return half_power * half_power;
    } else {
        p--;
        int half_power = power(n, p / 2);
        return half_power * half_power * n;
    }
}
```

Important:

```cpp
int half_power = power(n, p / 2);
```

stores the recursive result so that the recursive call is made only once.

---

# 4. Time Complexity of Recursive Functions

The main idea is the same as for normal functions:

> Count all operations performed by the original function call and every function call caused by it.

For recursive functions, it is often useful to draw a **recursion tree**.

---

## 4.1 Power Variation 1

```cpp
power(n, p)
```

calls:

```text
power(n, p)
power(n, p-1)
power(n, p-2)
...
power(n, 1)
```

There are approximately:

```text
p
```

function calls.

Each function call performs only constant work:

```text
O(1)
```

Therefore:

```text
p × O(1)
```

so:

```text
O(p)
```

---

## 4.2 Power Variation 2

Each recursive call divides `p` by 2:

```text
p
p/2
p/4
p/8
...
1
```

The number of times we can divide `p` by 2 until reaching 1 is:

```text
log₂(p)
```

Each function call performs constant work.

Therefore:

```text
O(log p)
```

---

## 4.3 Why Calling the Recursive Function Twice Changes Everything

Consider:

```cpp
return power(n, p / 2) * power(n, p / 2);
```

Instead of:

```cpp
int half_power = power(n, p / 2);
return half_power * half_power;
```

Now every call creates **two recursive calls**.

Recursion tree:

```text
                p
             /     \
           p/2     p/2
          /  \     /  \
        p/4 p/4  p/4 p/4
```

At level `i`:

```text
2^i
```

function calls exist.

The tree has about:

```text
log₂(p)
```

levels.

Total calls are approximately:

```text
1 + 2 + 4 + ... + p/2
```

which is:

```text
p - 1
```

Therefore:

```text
O(p)
```

So:

```cpp
int half = power(...);
return half * half;
```

is much better than:

```cpp
return power(...) * power(...);
```

---

# 5. Tail Recursion

A function is **tail recursive** when the recursive call is the final operation performed by the function.

Example:

```cpp
int sum(int n, int total) {
    if (n == 0) {
        return total;
    }

    return sum(n - 1, total + n);
}
```

The recursive call:

```cpp
sum(n - 1, total + n)
```

is the last thing the function does.

Compare this with:

```cpp
int sum(int n) {
    if (n == 0) return 0;

    return n + sum(n - 1);
}
```

This is not tail recursive because after:

```cpp
sum(n - 1)
```

returns, the function still needs to perform:

```text
n + result
```

### Why Tail Recursion Matters

Some compilers/languages can perform **tail-call optimisation**.

Instead of keeping every recursive function call on the call stack, the compiler may reuse the same stack frame.

Conceptually:

```text
Normal recursion:
call
  call
    call
      call

Tail-call optimised recursion:
call
replace
replace
replace
```

This can reduce stack memory usage.

However, tail-call optimisation is not guaranteed in C++.

---

# 6. Can Recursion Always Be Replaced?

In principle, recursive algorithms can be rewritten without recursion.

Why?

Because the computer already implements recursion using a **call stack**.

If we want, we can manually simulate that stack using:

```cpp
stack<...>
```

or other iterative state-management techniques.

So recursion is not more powerful than iteration in terms of what can be computed.

We still use recursion because many problems have naturally recursive structures:

```text
Trees
Graphs
Divide-and-conquer
Backtracking
Merge Sort
Quick Sort
Binary Search
```

Recursive code can often be shorter and easier to reason about.

---

# 7. Merge Sort

Merge Sort is a **divide-and-conquer** sorting algorithm.

The main idea is:

```text
Divide
↓
Recursively sort
↓
Merge
```

---

## 7.1 Recursive Step

Split the array into two halves:

```text
Original
[5, 2, 8, 1, 9, 4]

Left
[5, 2, 8]

Right
[1, 9, 4]
```

Recursively sort both halves:

```text
Left:
[2, 5, 8]

Right:
[1, 4, 9]
```

Then merge them:

```text
[1, 2, 4, 5, 8, 9]
```

---

## 7.2 Base Case

An array containing zero or one item is already sorted.

```cpp
if (n <= 1) return;
```

---

# 8. Merge Operation

Suppose:

```text
Left:
[1, 2, 5, 8, 9]

Right:
[3, 4, 6, 7, 10]
```

Compare the first unprocessed elements:

```text
1 vs 3
```

Take `1`.

Then:

```text
2 vs 3
```

Take `2`.

Then:

```text
5 vs 3
```

Take `3`.

Continue until one side becomes empty.

Then copy all remaining values from the other side.

This works because **both halves are already sorted**.

---

# 9. Merge Sort Implementation

```cpp
void merge_sort(int arr[], int n) {
    if (n <= 1) return;

    int left_half_size = n / 2;
    int right_half_size = n - left_half_size;

    int* left_half = new int[left_half_size];
    int* right_half = new int[right_half_size];

    for (int i = 0; i < n; i++) {
        if (i < left_half_size) {
            left_half[i] = arr[i];
        } else {
            right_half[i - left_half_size] = arr[i];
        }
    }

    merge_sort(left_half, left_half_size);
    merge_sort(right_half, right_half_size);

    int left_half_index = 0;
    int right_half_index = 0;
    int result_index = 0;

    while (
        left_half_index < left_half_size &&
        right_half_index < right_half_size
    ) {
        if (
            left_half[left_half_index] <
            right_half[right_half_index]
        ) {
            arr[result_index] =
                left_half[left_half_index];

            left_half_index++;
        } else {
            arr[result_index] =
                right_half[right_half_index];

            right_half_index++;
        }

        result_index++;
    }

    while (left_half_index < left_half_size) {
        arr[result_index++] =
            left_half[left_half_index++];
    }

    while (right_half_index < right_half_size) {
        arr[result_index++] =
            right_half[right_half_index++];
    }

    delete[] left_half;
    delete[] right_half;
}
```

---

# 10. Merge Sort Time Complexity

Each call splits the input approximately in half:

```text
n
↓
n/2
↓
n/4
↓
n/8
↓
...
↓
1
```

Therefore the recursion tree has:

```text
log₂(n)
```

levels.

At every level, all subproblems together process approximately all `n` items.

For example:

### Level 0

```text
1 × n = n
```

### Level 1

```text
2 × n/2 = n
```

### Level 2

```text
4 × n/4 = n
```

So every level performs:

```text
O(n)
```

work.

There are:

```text
O(log n)
```

levels.

Therefore:

```text
O(n) × O(log n)
```

gives:

```text
O(n log n)
```

### Merge Sort Complexity

```text
Best case:    O(n log n)
Average case: O(n log n)
Worst case:   O(n log n)
```

---

# 11. Uneven Merge Sort: 1/3 and 2/3

Suppose Merge Sort splits into:

```text
n/3
and
2n/3
```

instead of equal halves.

The longest recursive branch repeatedly keeps:

```text
2n/3
```

elements.

So:

```text
n
2n/3
(2/3)^2 n
...
1
```

The recursion depth is still logarithmic:

```text
O(log n)
```

Each level still processes approximately `n` total items.

Therefore:

```text
O(n log n)
```

The constant associated with the logarithm changes, but Big-O ignores constant factors.

---

# 12. Three-Way Merge Sort

Instead of two parts:

```text
n/2 + n/2
```

we could create three parts:

```text
n/3 + n/3 + n/3
```

The recursion depth becomes:

```text
log₃(n)
```

At every level, the total amount of data processed is still:

```text
O(n)
```

Therefore:

```text
O(n log₃ n)
```

Since logarithm bases differ only by a constant:

```text
O(n log n)
```

---

# 13. Quick Sort

Quick Sort is another divide-and-conquer sorting algorithm.

The key difference from Merge Sort is:

### Merge Sort

```text
Split arbitrarily
↓
sort halves
↓
merge carefully
```

### Quick Sort

```text
Partition carefully
↓
sort partitions
↓
no merge required
```

Quick Sort first arranges the array so that:

```text
values <= pivot
pivot
values > pivot
```

Then recursively sorts the two sides.

---

# 14. Pivot

A **pivot** is the comparison value used to partition the array.

Example:

```text
[3, 20, 2, 1, 8, 6, 19, 4]
```

Suppose pivot is:

```text
4
```

After partitioning:

```text
[3, 2, 1]  4  [20, 8, 6, 19]
```

Now we know:

```text
everything on the left <= 4
everything on the right > 4
```

The pivot is already in its final sorted position.

Therefore the recursive calls do not need to include it.

---

# 15. Quick Sort Structure

```cpp
void quick_sort(
    int arr[],
    int start_index,
    int end_index
) {
    if (end_index <= start_index) {
        return;
    }

    int pivot_index =
        partition(arr, start_index, end_index);

    quick_sort(
        arr,
        start_index,
        pivot_index - 1
    );

    quick_sort(
        arr,
        pivot_index + 1,
        end_index
    );
}
```

Base case:

```cpp
if (end_index <= start_index) return;
```

means the range contains zero or one element.

---

# 16. In-Place Partitioning

A convenient partition implementation uses:

```cpp
int right_partition_start = start_index + 1;
```

Its meaning is:

> If a right partition exists, point to its first item. Otherwise, point to the next position where the right partition would begin.

Implementation:

```cpp
int partition(
    int arr[],
    int start_index,
    int end_index
) {
    int pivot_index = start_index;
    int pivot_value = arr[pivot_index];

    int right_partition_start =
        start_index + 1;

    for (
        int i = start_index + 1;
        i <= end_index;
        i++
    ) {
        if (arr[i] <= pivot_value) {
            swap(
                arr[i],
                arr[right_partition_start]
            );

            right_partition_start++;
        }
    }

    swap(
        arr[pivot_index],
        arr[right_partition_start - 1]
    );

    pivot_index =
        right_partition_start - 1;

    return pivot_index;
}
```

Important invariant during the loop:

```text
pivot | left partition | right partition | unseen items
```

where:

```text
left partition  <= pivot
right partition > pivot
```

---

# 17. Quick Sort Best Case

The best case happens when the pivot repeatedly splits the array into approximately equal halves:

```text
n
↓
n/2 + n/2
↓
n/4 + n/4 + n/4 + n/4
```

The recursion tree has:

```text
O(log n)
```

levels.

Partitioning across each level processes:

```text
O(n)
```

items.

Therefore:

```text
O(n log n)
```

---

# 18. Quick Sort Worst Case

The worst case happens when the pivot is repeatedly the smallest or largest element.

Then:

```text
Q(n)
↓
Q(n-1)
↓
Q(n-2)
↓
Q(n-3)
↓
...
↓
Q(1)
```

The work is approximately:

```text
n + (n-1) + (n-2) + ... + 1
```

Using:

```text
1 + 2 + ... + n
=
n(n+1)/2
```

we get:

```text
O(n²)
```

### Quick Sort Complexity

```text
Best case:    O(n log n)
Average case: O(n log n)
Worst case:   O(n²)
```

---

# 19. Why Quick Sort is Usually Still Fast

Quick Sort does not need perfectly equal partitions.

Suppose every split is:

```text
1% / 99%
```

The longest branch is:

```text
n
0.99n
0.99²n
0.99³n
...
1
```

This still has logarithmic depth:

```text
O(log n)
```

because each recursive step reduces the input by a **constant factor**.

Therefore the total complexity is still:

```text
O(n log n)
```

The problematic case is when the reduction is only by a **constant amount**:

```text
n
n - 1
n - 2
n - 3
...
```

This gives:

```text
O(n²)
```

Important distinction:

```text
Multiply by constant factor each step
→ logarithmic depth

Subtract constant amount each step
→ linear depth
```

---

# 20. Merge Sort vs Quick Sort

| Property | Merge Sort | Quick Sort |
|---|---|---|
| Main idea | Split, sort, merge | Partition, then sort |
| Best time | O(n log n) | O(n log n) |
| Average time | O(n log n) | O(n log n) |
| Worst time | O(n log n) | O(n²) |
| Extra arrays in this implementation | Yes | No |
| In-place | Usually no | Yes |
| Memory advantage | Lower | Higher |
| Pivot needed | No | Yes |

Quick Sort is called **in-place** because it rearranges the original array rather than creating separate sub-arrays.

Note: recursive Quick Sort still uses call-stack memory, so "in-place" does not mean literally zero extra memory.

---

# 21. Core Patterns to Remember

## Pattern 1: Subtract a Constant

```text
n
n-1
n-2
...
1
```

Number of recursive levels:

```text
O(n)
```

Example:

```cpp
power(n, p - 1);
```

---

## Pattern 2: Divide by a Constant Factor

```text
n
n/2
n/4
n/8
...
1
```

Number of recursive levels:

```text
O(log n)
```

Example:

```cpp
power(n, p / 2);
```

---

## Pattern 3: Two Half-Size Recursive Calls

```text
T(n) = 2T(n/2) + O(1)
```

Total number of calls is:

```text
O(n)
```

Example:

```cpp
power(n, p/2) * power(n, p/2);
```

---

## Pattern 4: Two Half-Size Calls + Linear Work

```text
T(n) = 2T(n/2) + O(n)
```

Each level costs:

```text
O(n)
```

and there are:

```text
O(log n)
```

levels.

Therefore:

```text
O(n log n)
```

Example:

```text
Merge Sort
Quick Sort best case
```

---

# 22. Fibonacci Exercise

A simple recursive Fibonacci implementation is:

```cpp
int fibonacci(int n) {
    if (n <= 1) {
        return n;
    }

    return fibonacci(n - 1)
         + fibonacci(n - 2);
}
```

The recursion tree branches repeatedly:

```text
fib(n)
├── fib(n-1)
│   ├── fib(n-2)
│   └── fib(n-3)
└── fib(n-2)
    ├── fib(n-3)
    └── fib(n-4)
```

The number of calls grows exponentially.

A simple upper-bound analysis gives:

```text
O(2^n)
```

A tighter bound is related to:

```text
O(φ^n)
```

where `φ` is the golden ratio.

For this course, `O(2^n)` is usually the important intuition:

> Repeated overlapping recursive calls can cause exponential growth.

---

# 23. Recursive Sum of Even Numbers

One possible implementation:

```cpp
int sum_even(
    int arr[],
    int start,
    int end
) {
    if (start == end) {
        return 0;
    }

    int current = 0;

    if (arr[start] % 2 == 0) {
        current = arr[start];
    }

    return current
         + sum_even(arr, start + 1, end);
}
```

Example:

```text
[2, 5, 8, 3]

2 + sum([5, 8, 3])
2 + 0 + sum([8, 3])
2 + 0 + 8 + sum([3])
2 + 0 + 8 + 0
= 10
```

Time complexity:

```text
O(n)
```

because one recursive call processes one array element.

---

# 24. Exam / Worksheet Checklist

When analysing a recursive algorithm, ask:

1. What is the **input size**?
2. What is the **base case**?
3. How does each recursive call reduce the input?
4. How many recursive calls does each call create?
5. What work happens **outside** the recursive calls?
6. How many levels are in the recursion tree?
7. How much work occurs at each level?
8. Add the work across all levels.
9. Simplify to Big-O.

Useful clues:

```text
n → n - 1
usually suggests O(n) depth

n → n / 2
usually suggests O(log n) depth

two branches
can dramatically increase total calls

O(n) work at each of O(log n) levels
→ O(n log n)
```
