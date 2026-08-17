# Week 3 Time Complexity and Big-O Notation
## 1. Why do we need analyse complexity?
When evaluating a program, we usually ask:
- does it compile?
- does it produce the correct result?
- Is the code readable?

However, we also need to considerate efficiency.

Two important questions are:
- How much time does the algorithm take?
- How much memory does the algorithm use?

In Data Structures and Algorithms, we often care about how well an algorithm performs when the input becomes very large.

For example, two sorting algorithms may both correctly sort an array, but one may take 3 seconds while another takes 3 hours for the same large input.

Therefore, correctness alone is not enough. We also need to analyse the resources required by an algorithm.

## 2. Number of Operations
Instead of measuring the exact running time of a program, we could estimate
its performance by counting the number of basic operations it performs.

Examples of basic operation include:
```C++
x = 5; // assignment
x++; // increment
x > 10; // comparison
return x // return
```

The general idea is:
```txt
More operations
      ↓
More execution time
```
Therefore, the number of operations can be used as a proxy for running time.

## 3. What is T(n)?
Consider:
```C++
int sum(int arr[], int n) {
    int result = 0;

    for (int i = 0; i < n; i++) {
        result += arr[i];
    }

    return result;
}
```
The number of operations depends on the size of the input array

If an array contains:
```txt
10 elements      → loop runs 10 times
100 elements     → loop runs 100 times
1,000 elements   → loop runs 1,000 times
```
We can define:
T(n) as:
```txt
The number of operations performed by
the algorithm when the input size is n.
```

For example:
T(n) = 4n + 3
The exact numbers may depend on how we count operations, 
but the important idea is that the number of operations grows
with n.

## 4. Always Define the Input Size
When performing complexity analysis, we should clearly define what n
represents.

For example:
```C++
int sum(int arr[], int n)
```

we could say:
> Let n be the number of elements in the input array.

This is important because n in complexity analysis represents 
the input size.

It doesn't have to be the variable named n in the program.

For example:
```C++
void func(int m) {
    for (int i = 0; i < m; i++) {
        cout << "hi";
    }
}
```

We could write:
T(m)
where m is the input integer.

## 5. Multiple Input Sizes
Sometimes an algorithm depends on more than one input.

Example:
```C++
int char_count(string x, string y, char target)
```

If:
- n = length of x
- m = length of y

Then, we may write:
T(n,m)

For example:
T(n, m) = 4*n + 4*m + 4

The complexity depends on both inputs.

## 6. Complexity must be based on input, Not output
Consider:
```C++
void func(int n) {
    int x = 0;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            x++;
        }
    }
}
```
It would be incorrect to say;
>  n is the final value of x

The purpose of complexity analysis is to determine how much work the algorithm performs based on the input.

The outer loop runs n times.

For every outer-loop iteration, the inner loop also runs n times.

Therefore:

n times n=n^2

The algorithm performs a number of operations proportional to:

n^2

## 7. Asymptotic Analysis
Complexity analysis is mainly concerned with what happens when the input becomes very large.

We are interested in:

n -> infinity

This is called asymptotic analysis.

We are not mainly interested in whether an algorithm is fast when:

```txt
n = 1
n = 5
n = 10
```

Although every algorithm is very small inputs。

Instead, we want to understand:
> How quickly does the amount of work grow as the input size increases

## 8. Big-O Notation
Big-O notations describe the growth rate of an algorithm.
Suppose:
T(n) = 3*n**2 + 12*n + 3

To find the Big-O complexity:

### Step 1: Keep the dominant term

The term are:
3*n**2, 12*n, 3

As n become very large:

n**2 

grows much faster than:

n

Or a constant.

Therefore:

3*n**2 + 12*n + 3

becomes approximately dominated by:

3*n**2

### Step 2: Remove the constant factor
Big-O ignores constant factors.

Therefore:

3n^2

becomes

n^2

So:

O(n^2)

## 9. Dominant Terms
When finding Big-O, We normally keep only the fastest-growing term.

For, example:

T(n)=7n^3+20n^2+100n+50

The dominant term is:

n^3

Therefore:

O(n^3)

Another example:

T(n)=5n^2+100000n+999999

The dominant term is:

n^2

Therefore:

O(n^2)

Even though 100000n may look large, n² eventually grows much faster.

## 10. Why do we ignore constants?
Consider:

T_1(n)=n

and:

T_2(n)=10000n

The second algorithm performs much more work for the same n.

However, both have the same growth behaviour.

If the input doubles:
```txt
n → 2n
```
then both operation counts also approximately double.

Therefore both belong to:

O(n)

Big-O is mainly concerned with the rate of growth, not the exact number of operations.

## 11. Common Complexity Classes

From fastest-growing efficiency to slowest:
```txt
O(1)
O(log n)
O(n)
O(n log n)
O(n²)
O(n³)
O(2ⁿ)
```
In general:
```txt
O(1)       → excellent scalability
O(log n)   → very good
O(n)       → good
O(n log n) → common in efficient sorting
O(n²)      → expensive for large inputs
O(2ⁿ)      → extremely expensive
```
## 12. O(1) — Constant Time
An algorithm is O(1) when the number of operations does not grow with the input size.

Example:
```C++
int get_middle_item(int arr[], int size) {
    return arr[size / 2];
}
```

Whether the array contains:

```txt
10 elements
1,000 elements
1,000,000 elements
```

the algorithm directly accesses one element.

Therefore:

O(1)

This is possible because arrays support random access.

## 13. O(log n) — Logarithmic Time
A common pattern for O(log n) is when the problem size is repeatedly divided by a constant.

Example:

```C++
while (n>1)
{
    n = n/2;
}
```
Suppose:
```txt
n = 32
```

The values become:
```txt
32
16
8
4
2
1
```

The loop runs 5 times.

Because:

2^5=32

we have:

log_2 32=5

Therefore, the time complexity is:

O(log(n))

## 14. Binary Search

Binary search is a classic O(log n) algorithm.

It works on a sorted array.

Instead of checking every element, 
it checks the middle element and removes 
half of the remaining search space.

For example:
```txt
16 elements
↓
8 elements
↓
4 elements
↓
2 elements
↓
1 element
```

The search space follows:

n, n/2, n/4, n/8, ...

Therefore, the number of steps is approximately:

log_2 n

So the worst-case time complexity is:

O(log n)

## 15. Why Do We Ignore the Base of log?
We normally write:

O(log n)

instead of:

O(log_2 n)

or:

O(log_10 n)

This is because changing the algorithm base only introduces
a constant factor.

Using the change-of-base formula:

log_a n = log_b n/ log_b a

The value:

1/logb_a

is a constant.

Since Big-O ignores constant factors, all constant
logarithm bases belong to:

O(log n)

## 16. O(n) -- Linear Time
Example:
```C++
int find_total(int arr[], int n) {
    int total = 0;

    for (int i = 0; i < n; i++) {
        total += arr[i];
    }

    return total;
}
```
The loop runs once for every element.

Therefore:

T(n) \propto n

and:

O(n)

If the input size doubles, the amount of work approximately doubles.

## 17. O(n log n) -- Superlinear Time
Suppose an algorithm performs a logarithmic operation for every element

Example:
```C++
for (int i = 0; i < n; i++) {
    binary_search(...);
}
```
The outer loop runs:

n

times.

Binary search costs:

O(log n)

Therefore:

n * log n

giving:

O(n log n)

Many efficient sorting algorithms have O (n log n) time complexity.

## 18. Quadratic Time
A common pattern is two nested loops that run n times.

Example:
```C++
for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
        cout << "hi";
    }
} 
```

The outer loop runs:

n

times.

Therefore:

n*n = n**2

So:

O(n²)

If the input doubles:

n -> 2n

Then

n**2 -> (2n)**2 = 4*n**2

Therefore, the amount of work approximately quadruples.

## 19. Two Nested Loop Do not Always Mean O(n²)
This is important

Consider:
```C++
for (int i = 1; i < n; i *= 2) {
    for (int j = 0; j < n; j++) {
        cout << "hi";
    }
}
```
The outer loop follows:
```txt
1
2
4
8
16
...
```
Therefore, it runs:
O(log n)
times.

Therefore, the total complexity is:
O(n log n)
So we should never simply say
> Two loops means  O(n²)

Instead, determine how many times each loop actually runs.

## 20. A Loop That Runs Up to i
Consider:
```C++
for (int i = 0; i < n; i++) {
    for (int j = 0; j < i; j++) {
        cout << "hi";
    }
}
```
The inner loop does not always run n times.

Instead:
```txt
i = 0 → 0 iterations
i = 1 → 1 iteration
i = 2 → 2 iterations
i = 3 → 3 iterations
...
i = n - 1 → n - 1 iterations
```

The total is:

0+1+2+ ... +(n-1)

Using the arithmetic-series formula:

n*(n-1) / 2

Expand it :

(n**2 - n) / 2

The dominant term is:

n**2

Therefore:

O(n²)

## 21. Sequential Loops vs Nested Loops
The distinction is extremely important.

**Sequential Loops**

```C++
for (int i = 0; i < n; i++) {
    ...
}

for (int j = 0; j < n; j++) {
    ...
}
```

This work is:

n + n = 2n

Therefore:

O(n)

**Nested Loops**
```C++
for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
        ...
    }
}
```

This work is:

n x n = n**2

Therefore:

O(n²)

A useful rule is:
> Sequential work usually adds. Nested work usually multiplies.

But always check how many times the loops actually run.

## 22.  Best Case and Worst Case
Consider linear search:
```C++
bool search(int arr[], int n, int target) {
    for (int i = 0; i < n; i++) {
        if (arr[i] == target) {
            return true;
        }
    }

    return false;
}
```

Best Case 
If the target is the first element:
```C++
bool search(int arr[], int n, int target) {
    for (int i = 0; i < n; i++) {
        if (arr[i] == target) {
            return true;
        }
    }

    return false;
}
```

**Best Case**:

If the target is the first element：
```C++
arr[0] == target
```
the algorithm stops immediately.

Therefore:

O(1)

**Worst Case**

if the target is:
- the last element, or
- not in the array

the algorithm may inspect every element.

Therefore:

O(n)

A good answer would be:

> The best-case time complexity is O(1) when the target is the first 
> element. The worst-case time complexity is O(n) when the target is at the end of the array or does not exist.

## 23. A small input is not a best case 
Consider:
```C++
int sum(int arr[], int n) {
    int result = 0;

    for (int i = 0; i < n; i++) {
        result += arr[i];
    }

    return result;
}
```

It would not be useful to say:
> he best case occurs when n = 0.

Complexity analysis is concerned with how the algorithm behaves as the input size grows.

For any array of size n, the algorithm must inspect all n elements.

Therefore, there is no meaningful difference between best and worst casees.

The complexity is always:

O(n)

## 24. Optimized Bubble Sort
Consider:
```C++
void bubble_sort(int arr[], int n) {
    for (int pass = 0; pass < n; pass++) {

        bool had_swap = false;

        for (int i = 0; i < n - 1; i++) {
            if (arr[i] > arr[i + 1]) {
                swap(arr[i], arr[i + 1]);
                had_swap = true;
            }
        }

        if (!had_swap) {
            break;
        }
    }
}
```

**Worst Case**

The outer loop may run approximately n times.

The inner loop may run approximately n times.

Therefore:

n*n = n**2

So:

O(n**2)

**Best Case**

If the array is already sorted, the first pass performs no swaps.

The algorithm then immediately breaks.

Only one complete scan of the array is required.

Therefore:

O(n)

So:

```txt
Best case:  O(n)
Worst case: O(n²)
```

## 25. Array Random Access
Suppose:
```C++
arr[56];
```
Arrays store elements consecutively in memory.

The computer can calculate the address directly:

```txt
base address
+
56 × size of each element
```

Therefore, accessing any valid array index takes a constant amount of work.

So array random access is:

O(1)

## 26. Linked List Access

A linked list does not store all nodes consecutively.

For example:
```txt
first
  ↓
Node 0
  ↓
Node 1
  ↓
Node 2
  ↓
...
  ↓
Node n
```

To reach index n, we must start from the first node and follow next pointers.

For example:
```C++
Node* current = first;

for (int i = 0; i < index; i++) {
    current = current->next;
}
```
If the requested index is n, approximately n nodes must be traversed.

Therefore:

O(n)

where n is the index being accessed.

This is one major disadvantage of linked lists compared with arrays

## 27. Why Is Linked-List Access Not Best Case O(1)?Why Is Linked-List Access Not Best Case O(1)?

It is true that accessing index 0 takes constant time.

However, if we define n as the index being accessed, then 
increasing n increases the number of nodes must be traversed.

For example:
```txt
index 1       → approximately 1 step
index 100     → approximately 100 steps
index 1000000 → approximately 1000000 steps
```

Therefore:

T(n)\propto n

and the access operation is:

O(n)

The fact that one particular input such as n = 0 is fast does not change its asymptotic growth rate.

## 28. A Practical Method for Complexity Analysis
When analysing code, use the following process.

**Step 1: Define the input**

Example:

> Let n be the number of elements in the input array.

**Step 2: Determine how many times each loop runs**

Ask whether it runs:
```txt
constant times
n times
n / 2 times
log n times
i times
```

**Step 3: Check whether operations are sequential or nested**

Sequential:
n+n=2n=O(n)

Nested:
n * n=n^2=O(n^2)

**Step 4: Check for early termination**
Look for the statement such as:
```C++
return;
break;
```
These may create different:
- best cases
- worst cases

**Step 5: Find the dominant term**
Example:

T(n)=5n^2+10n+20

Dominant term:

n^2

Therefore:

O(n^2)

## Complexity Summary
```txt
Complexity   Name    Typical Pattern

O(1)

Constant

Direct array access
------------------------------------
O(log n) 

Logarithmic

Repeatedly halve the problem
--------------------------------------
O(n)

Linear

Traverse all elements once
----------------------------------------
O(n log n)

Superlinear

n logarithmic operations
---------------------------------------
O(n²)

Quadratic

Two linear nested loops
-----------------------------------------
O(2ⁿ)

Exponential

Explore all subsets / possibilities
```

A good principle is:
> Time complexity describes how the amount of work performed by an algorithm grows as the input size increases.

