# FIT1008 / FIT1054 / FIT2085 --- Week 6: Algorithm Correctness and Loop Invariants

## 1. Why Algorithm Correctness Matters

Testing can show that an algorithm works for the test cases we selected,
but it does not prove that the algorithm works for every valid input.

A **correctness proof** gives a logical argument showing why an
algorithm produces the required result for all valid inputs.

### Testing vs Proof

-   **Testing:** provides evidence that the program works on particular
    inputs.
-   **Proof:** establishes why the algorithm works for all inputs
    satisfying its assumptions.

A program may pass many tests while still containing an edge-case error.

------------------------------------------------------------------------

## 2. Simple Correctness Arguments

For code without loops, correctness can often be shown by making a
sequence of small claims.

Example:

``` cpp
int frame_area(int w, int h, int b) {
    int inside_area = w * h;
    int outside_area = (w + 2*b) * (h + 2*b);
    return outside_area - inside_area;
}
```

Reasoning:

1.  `inside_area = w * h` is the area of the photo.
2.  The outside rectangle has width `w + 2b` and height `h + 2b`.
3.  Therefore, `outside_area` is the area of the photo and frame
    together.
4.  Subtracting the photo area from the outside area gives the frame
    area.

A good proof uses small, explicit logical steps rather than vague
statements.

------------------------------------------------------------------------

## 3. Preconditions

A correctness argument normally applies to **valid inputs**.

A **precondition** describes something that must be true before the
algorithm is executed.

For the frame example, reasonable preconditions may include:

``` text
w > 0
h > 0
b >= 0
```

The exact preconditions depend on the specification.

------------------------------------------------------------------------

## 4. The P(k) Idea

Loops are harder to prove because the same code may execute many times.

A useful approach is to define a statement `P(k)` describing what is
true after some amount of progress.

Example for finding the largest frame:

> **P(k):** `largest_frame` contains the largest frame area among the
> first `k` frames.

Then:

``` text
P(0)
 ↓
P(1)
 ↓
P(2)
 ↓
...
 ↓
P(n)
```

To reach `P(n)`, we show:

1.  `P(0)` is initially true.
2.  If `P(k)` is true, executing the next iteration makes `P(k+1)` true.

This is similar to mathematical induction.

------------------------------------------------------------------------

## 5. Array Slice Notation

Proofs often use:

``` text
arr[a:b]
```

This means the elements starting at index `a` and ending at index
`b - 1`.

The left boundary is **inclusive** and the right boundary is
**exclusive**.

Examples:

``` text
arr[0:3] → indices 0, 1, 2
arr[1:5] → indices 1, 2, 3, 4
arr[0:n] → the entire array
arr[6:6] → empty range
```

------------------------------------------------------------------------

## 6. Loop Invariants

A **loop invariant** is a property that is guaranteed to be true at a
specified point of every relevant loop iteration.

Example:

> At the beginning of each iteration, `arr[0:i]` is sorted.

The values of the variables may change, but the relationship described
by the invariant continues to hold.

### The Location Matters

An invariant must state **where** it is true.

For example:

``` text
At the beginning of each iteration...
```

and:

``` text
At the end of each iteration...
```

may require different invariant statements.

A statement does not need to be true at every line inside the loop.

------------------------------------------------------------------------

# 7. The Three Parts of an Invariant Proof

Every invariant proof should clearly contain:

## 7.1 Initialisation

Show that the invariant is true the **first time** it is supposed to
apply.

Question to ask:

> Is the invariant true before/after the first relevant iteration?

------------------------------------------------------------------------

## 7.2 Maintenance

Assume the invariant is true at one iteration and prove that executing
the loop preserves it for the next iteration.

A common structure is:

> Let `k` be a fixed iteration value. Assume the invariant holds when
> the loop variable is `k`.

Then show:

``` text
Invariant at k
+
one iteration of the algorithm
↓
Invariant at k + 1
```

Using a constant such as `k` makes the reasoning precise.

------------------------------------------------------------------------

## 7.3 Termination

Connect the invariant to the final goal of the algorithm.

Question to ask:

> When the loop terminates, does the invariant imply exactly what the
> function was supposed to achieve?

An invariant can be true but still be useless if it does not establish
the required result at termination.

------------------------------------------------------------------------

# 8. Insertion Sort

``` cpp
void insertion_sort(int arr[], int n) {
    for (int first_unsorted_index = 1;
         first_unsorted_index < n;
         first_unsorted_index++) {

        int current_index = first_unsorted_index;

        while (current_index - 1 >= 0 &&
               arr[current_index - 1] > arr[current_index]) {

            swap(arr[current_index - 1], arr[current_index]);
            current_index--;
        }
    }
}
```

Insertion sort maintains a sorted prefix and inserts one new element
into that prefix during each outer-loop iteration.

The loop begins at index `1` because a single element at index `0` is
already trivially sorted.

------------------------------------------------------------------------

## 9. Insertion Sort Invariant

One useful invariant is:

> **At the end of each iteration of the outer `for` loop,
> `arr[0:first_unsorted_index + 1]` is sorted in non-descending order.**

If:

``` text
first_unsorted_index = 3
```

then:

``` text
arr[0:4]
```

contains indices `0, 1, 2, 3`, including the element processed during
that iteration.

### Initialisation

The first iteration has:

``` text
first_unsorted_index = 1
```

The algorithm considers `arr[0]` and `arr[1]`.

-   If `arr[0] <= arr[1]`, no swap is necessary.
-   If `arr[0] > arr[1]`, they are swapped.

In either case:

``` text
arr[0:2]
```

is sorted, so the invariant holds.

### Maintenance

Suppose the invariant holds at the end of an iteration with:

``` text
first_unsorted_index = k
```

Therefore:

``` text
arr[0:k+1]
```

is sorted.

In the next iteration, the new element at index `k+1` is repeatedly
swapped left while it is smaller than the element immediately before it.

The previously sorted elements retain their relative order.

When the `while` loop terminates:

-   either the new element has reached index `0`, or
-   it is greater than or equal to the element immediately to its left.

Every element that the new element previously passed was greater than
it.

Therefore the new element has been inserted into the correct position
and:

``` text
arr[0:k+2]
```

is sorted.

Thus the invariant is maintained.

### Termination

The final outer-loop iteration has:

``` text
first_unsorted_index = n - 1
```

The invariant therefore gives:

``` text
arr[0:n]
```

is sorted.

Since `arr[0:n]` is the whole array, insertion sort correctly sorts the
array.

------------------------------------------------------------------------

# 10. Strong and Weak Invariants

An invariant being **true** is not enough.

It must contain enough information to support the proof.

Consider selection sort:

``` cpp
void selection_sort(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        int next_smallest_index =
            smallest_item_index(arr, n, i);

        swap(arr[i], arr[next_smallest_index]);
    }
}
```

A tempting invariant is:

> `arr[0:i]` is sorted.

This is true during selection sort, but by itself it is too weak for the
Maintenance proof.

Knowing only that:

1.  `arr[0:k]` is sorted, and
2.  the minimum of `arr[k:n]` is moved to position `k`

does **not** logically guarantee that `arr[0:k+1]` is sorted.

Example:

``` text
[1, 2, 4, 7, 6, 5, 3, 8]
```

For `k = 4`:

``` text
arr[0:4] = [1, 2, 4, 7]
```

is sorted.

The minimum of:

``` text
arr[4:8] = [6, 5, 3, 8]
```

is `3`.

Putting `3` at index `4` gives:

``` text
[1, 2, 4, 7, 3, 5, 6, 8]
```

and:

``` text
arr[0:5] = [1, 2, 4, 7, 3]
```

is not sorted.

The actual selection-sort execution would never reach this particular
state, but the weak invariant does not contain enough information to
exclude it.

------------------------------------------------------------------------

## 11. Better Selection Sort Invariants

Useful stronger invariants include:

> `arr[0:i]` is sorted, and every element in `arr[0:i]` is less than or
> equal to every element in `arr[i:n]`.

or:

> `arr[0:i]` contains the `i` smallest values in the array, in sorted
> order.

The second form is particularly useful.

If `arr[0:k]` already contains the `k` smallest values, then the minimum
of `arr[k:n]` must be the next-smallest value.

After placing it at index `k`, `arr[0:k+1]` contains the `k+1` smallest
values in sorted order.

------------------------------------------------------------------------

# 12. Properties of a Good Invariant

A useful invariant should satisfy three main requirements.

### 1. It uses program variables precisely

Prefer:

> `count` equals the number of qualifying elements among `arr[0:i]`.

rather than:

> `count` contains how many qualifying elements we have seen so far.

### 2. It is strong enough for Maintenance

The invariant must contain all information needed to logically prove
that the next iteration preserves it.

Avoid relying on hidden facts that are not stated or implied by the
invariant.

### 3. It is useful for Termination

When the loop finishes, the invariant should imply the algorithm's
required result.

A statement such as:

``` text
i >= 0
```

may be invariant, but it does not prove that a sorting algorithm sorts
anything.

------------------------------------------------------------------------

# 13. Not Every Invariant Is About the First k Elements

For algorithms that process an array from left to right, a statement
about the first `k` elements is often natural.

Examples:

### Sum

> `sum` equals the sum of the first `k` elements.

### Count

> `count` equals the number of qualifying elements among the first `k`
> elements.

### Maximum

> `maximum` equals the maximum among the first `k` elements.

However, some algorithms progress differently.

------------------------------------------------------------------------

## 14. In-Place Array Reversal

Suppose:

``` text
[1, 2, 3, 4, 5, 6]
```

is reversed by swapping opposite ends:

``` text
[6, 2, 3, 4, 5, 1]
[6, 5, 3, 4, 2, 1]
[6, 5, 4, 3, 2, 1]
```

Each iteration completes work at **both ends** of the array.

Therefore an invariant only describing the first `k` elements does not
naturally describe all progress.

A better invariant is:

> The first `k` elements and the last `k` elements are in their correct
> reversed positions.

The invariant should reflect how the algorithm actually progresses.

------------------------------------------------------------------------

# 15. Binary Search and Invariants

For ordinary binary search, invariants describe what is known about the
current search boundaries.

One approach defines:

> `left` points to an index known to contain a value smaller than the
> target.

> `right` points to an index known to contain a value larger than the
> target.

The algorithm then updates `left` or `right` while maintaining these
properties.

This demonstrates an important idea:

> The implementation can often be designed around the invariants.

There can be multiple correct binary-search implementations depending on
how the boundaries and invariants are defined.

------------------------------------------------------------------------

# 16. Binary Search with Hypothetical Boundaries

One implementation uses:

``` cpp
int left = -1;
int right = n;
```

Conceptually:

-   index `-1` represents a hypothetical value smaller than the target;
-   index `n` represents a hypothetical value larger than the target.

Then:

``` cpp
while (right > left + 1) {
    int mid = (left + right) / 2;

    if (arr[mid] == target)
        return mid;

    if (arr[mid] > target)
        right = mid;
    else
        left = mid;
}
```

The boundary invariants are preserved until:

``` text
right = left + 1
```

At that point there is no actual array element remaining between the
known-smaller and known-larger boundaries. Therefore, if the target was
not already returned, it is absent.

------------------------------------------------------------------------

# 17. Binary Search for a Rotated Array Minimum

Consider a sorted array that has been rotated:

``` text
[4, 5, 6, 1, 2, 3]
```

The goal is to find the **index of the minimum element**.

There is no known target value, so ordinary invariants such as:

``` text
arr[left] < target
arr[right] > target
```

do not apply.

A natural invariant is:

> **The minimum element is somewhere within the current range
> `[left, right]`.**

Each iteration compares `arr[mid]` with a boundary such as `arr[right]`
to determine which part can safely be discarded.

For example:

``` text
[4, 5, 6, 1, 2, 3]
       ↑        ↑
      mid      right
```

Here:

``` text
arr[mid] = 6
arr[right] = 3
```

Since:

``` text
arr[mid] > arr[right]
```

the rotation point and minimum must be to the right of `mid`, so:

``` cpp
left = mid + 1;
```

can safely discard the left portion.

If instead:

``` text
arr[mid] < arr[right]
```

the minimum is at `mid` or to its left, so:

``` cpp
right = mid;
```

Notice that `mid` is retained because it could itself be the minimum.

The important property is maintained:

``` text
minimum remains inside [left, right]
```

When:

``` text
left == right
```

only one candidate remains. Since the invariant guarantees that the
minimum is inside the range, that index must contain the minimum.

------------------------------------------------------------------------

# 18. General View of Binary Search Invariants

The deeper idea behind binary search is:

> **The solution remains inside the current search space.**

Each iteration:

``` text
use known structure
        ↓
discard an impossible part
        ↓
preserve the solution
        ↓
reduce the search space
```

For ordinary binary search, the solution is the target index.

For rotated-array minimum search, the solution is the index of the
minimum element.

------------------------------------------------------------------------

# 19. Example: Counting Large Frames

``` cpp
int count_large_frames(
    int w_vals[],
    int h_vals[],
    int b_vals[],
    int n,
    int T
) {
    int count = 0;

    for (int i = 0; i < n; i++) {
        if (frame_area(w_vals[i], h_vals[i], b_vals[i]) >= T)
            count++;
    }

    return count;
}
```

A natural invariant at the **beginning** of each iteration is:

> `count` equals the number of frames with area at least `T` among the
> first `i` frames.

### Initialisation

Initially:

``` text
i = 0
count = 0
```

There are zero qualifying frames among the first zero frames, so the
invariant holds.

### Maintenance

Suppose at the beginning of an iteration `i = k`, `count` equals the
number of qualifying frames among the first `k` frames.

The algorithm examines frame `k`.

-   If its area is at least `T`, `count` is incremented.
-   Otherwise, `count` is unchanged.

In either case, after processing frame `k`, `count` equals the number of
qualifying frames among the first `k+1` frames.

When the next iteration begins with `i = k+1`, the invariant therefore
holds again.

### Termination

When the loop terminates:

``` text
i = n
```

The invariant states that `count` equals the number of qualifying frames
among the first `n` frames.

The first `n` frames are all frames, so the function returns the correct
count.

------------------------------------------------------------------------

# 20. Common Proof Mistakes

## Mistake 1: Vague wording

Avoid:

> the elements considered so far

Prefer:

> the elements in `arr[0:i]`

------------------------------------------------------------------------

## Mistake 2: Not specifying invariant location

Avoid simply writing:

> `arr[0:i]` is sorted.

Specify:

> At the beginning of each iteration, `arr[0:i]` is sorted.

------------------------------------------------------------------------

## Mistake 3: Invariant is true but too weak

A true statement does not automatically provide enough information for
Maintenance.

Selection sort's simple `"arr[0:i] is sorted"` invariant is the key
example.

------------------------------------------------------------------------

## Mistake 4: Hidden assumptions in Maintenance

Every conclusion must follow from:

1.  the invariant,
2.  the code executed during the iteration, and
3.  previously established facts/preconditions.

Do not rely on information that exists only implicitly in your
understanding of the algorithm.

------------------------------------------------------------------------

## Mistake 5: Forgetting Termination

Proving that an invariant remains true is not enough.

You must explicitly explain why its final state proves the function's
required result.

------------------------------------------------------------------------

## Mistake 6: Off-by-one errors

Always remember:

``` text
arr[a:b]
```

ends at:

``` text
b - 1
```

Therefore:

``` text
arr[0:i]     → first i elements
arr[0:i+1]   → indices 0 through i
arr[0:n]     → whole array
```

------------------------------------------------------------------------

# 21. Standard Proof Template

A useful exam template is:

## Invariant

> At the \[beginning/end\] of each iteration, ...

## Initialisation

> The first time the invariant is reached, the loop variable is ***. At
> this point, ***. Therefore, the invariant holds.

## Maintenance

> Let `k` be an integer in the valid loop range. Assume the invariant
> holds when the loop variable is `k`. Therefore, ***. During this
> iteration, the algorithm ***. Consequently, \_\_\_. Thus, when the
> invariant is reached again, it holds for the next iteration.

## Termination

> The loop terminates when ***. At this point, the invariant tells us
> ***. Since \_\_\_ represents the complete input / required result, the
> algorithm has achieved its goal.

Do not use this mechanically: the logical argument in Maintenance must
still be valid.

------------------------------------------------------------------------

# 22. Quick Checklist for Choosing an Invariant

Before proving anything, ask:

1.  **What does each important variable mean?**
2.  **Where in the loop should my invariant hold?**
3.  **What progress has definitely been completed at that point?**
4.  **Can I express that progress using loop variables?**
5.  **Is the statement true initially?**
6.  **Does it contain enough information to prove the next iteration?**
7.  **When the loop terminates, does it imply the algorithm's goal?**

If the answer to 5, 6, and 7 is yes, the invariant is likely useful.

------------------------------------------------------------------------

# 23. Key Ideas to Remember

``` text
Testing ≠ proof
```

``` text
Loop invariant
= a property that remains true
  at a specified point of the loop
```

``` text
Invariant proof:
1. Initialisation
2. Maintenance
3. Termination
```

``` text
Good invariant:
- precise
- uses code variables
- states where it holds
- strong enough for Maintenance
- useful for Termination
```

``` text
Maintenance:
assume invariant at k
        ↓
execute one iteration
        ↓
prove invariant for next iteration
```

``` text
Binary search:
the answer must remain inside
the current search space
```

Most importantly:

> **Do not choose an invariant merely because it is true. Choose one
> that contains enough information to prove the algorithm's progress and
> whose final state establishes the algorithm's goal.**
