# C1 Concepts — C++ Templates and Custom Operators

## 1. Why Templates Are Useful

Without templates, a function or data structure is often written for one fixed type.

For example:

```cpp
int get_max(int a, int b)
```

only works with `int`.

Templates allow us to write generic code that can work with different types.

```cpp
template <typename T>
T get_max(T a, T b)
```

Here, `T` is a placeholder for a type.

The compiler determines the actual type at compile time.

For example:

```cpp
get_max(3, 5);
```

means:

```text
T = int
```

while:

```cpp
get_max(2.5, 4.8);
```

means:

```text
T = double
```

---

## 2. Template Type Requirements

A template does not mean that every type will automatically work.

The type used as `T` must support the operations used inside the template.

For example:

```cpp
template <typename T>
T get_max(T a, T b) {
    if (a > b) {
        return a;
    }
    return b;
}
```

requires `T` to support the `>` operator.

Built-in types such as `int` and `double` already support comparison.

Custom types may need their own comparison operators.

---

## 3. Template Instantiation

Templates are resolved at compile time.

If a program calls:

```cpp
get_max<int>(3, 5);
```

the compiler creates a version of the function that works with `int`.

If the program also calls:

```cpp
get_max<double>(2.5, 4.8);
```

the compiler creates another version for `double`.

This process is called **template instantiation**.

---

## 4. Multiple Template Types

A template can use more than one generic type.

```cpp
template <typename T, typename U>
void example(T a, U b)
```

This allows the two parameters to have different types.

For example:

```cpp
example(10, 'A');
```

could use:

```text
T = int
U = char
```

---

## 5. Function Templates vs Struct Templates

Templates can be used with both functions and custom types.

Function template:

```cpp
template <typename T>
void print_value(T value)
```

Struct template:

```cpp
template <typename T>
struct Pair {
    T first;
    T second;
};
```

A struct template usually specifies the type when creating an object:

```cpp
Pair<int> p;
Pair<char> q;
```

---

## 6. Custom Operators

Built-in types already define operators such as:

```text
<
>
<=
==
+
-
```

For a custom type, C++ may not know what these operators should mean.

For example, if a `Rectangle` contains:

```cpp
int width;
int height;
```

C++ does not automatically know whether:

```cpp
a < b
```

should compare width, height, area, or something else.

We can define the behaviour ourselves using **operator overloading**.

---

## 7. Operator Overloading

A custom comparison operator can be defined inside a struct.

Example:

```cpp
bool operator<(const Rectangle& right) const
```

Conceptually:

```cpp
a < b
```

can be understood as:

```cpp
a.operator<(b)
```

The object on the left is the object calling the function.

The object on the right becomes the parameter.

---

## 8. `const Rectangle&`

A common operator parameter is:

```cpp
const Rectangle& right
```

This combines two ideas.

### Reference: `&`

Using a reference avoids copying the entire object.

Instead of creating a new copy, the parameter refers to the existing object.

### Const: `const`

`const` prevents the function from modifying the referenced object.

Therefore:

```cpp
const Rectangle&
```

means:

```text
Use the original object
without copying it
and do not modify it
```

---

## 9. The Final `const`

A comparison operator may also end with `const`.

```cpp
bool operator<(const Rectangle& right) const
```

The first `const` protects the object on the right.

The final `const` means the function does not modify the object on the left.

Therefore, a comparison function can safely compare both objects without changing either one.

---

## 10. Comparing Rectangles by Area

If rectangles are ordered by area, then comparison should use:

```text
area = width × height
```

For example:

```text
Rectangle A: 10 × 3 = 30
Rectangle B: 9 × 4 = 36
```

Therefore:

```text
A < B
```

is true because:

```text
30 < 36
```

---

## 11. Why `<` Is Needed

A generic sorting algorithm may use:

```cpp
a < b
```

to decide which element should come first.

If the array contains `Rectangle` objects, then `Rectangle` must define `<`.

The sorting algorithm itself does not need to know anything about width, height, or area.

It only depends on the comparison behaviour provided by the type.

---

## 12. Why `<=` Can Be Important

Merge sort often compares the next element from the left half with the next element from the right half.

Using:

```cpp
left <= right
```

means that if two elements are equal, the element from the left half is chosen first.

This helps preserve their original relative order.

That property is called **stability**.

For example:

```text
Original order:
A, B

A and B have equal area.
```

If the merge step chooses `A` first because it came from the left half, the order remains:

```text
A, B
```

A stable sorting algorithm preserves the relative order of equal elements.

---

## 13. Stable Sorting

A sorting algorithm is **stable** if equal elements remain in the same relative order after sorting.

Example:

```text
Before:
A(area 12), B(area 12)

After stable sort:
A(area 12), B(area 12)
```

A non-stable sort could produce:

```text
B(area 12), A(area 12)
```

Both results are correctly sorted by area, but only the first preserves the original order.

---

## 14. Generic Sorting

A generic sorting function replaces the fixed element type with `T`.

For example:

```cpp
template <typename T>
void insertion_sort(T arr[], int size)
```

Only values related to the array element type become `T`.

Indexes and sizes normally remain `int`.

For example:

```text
T     → array elements
int   → index
int   → size
```

---

## 15. Main Design Idea

The key idea is separation of responsibilities.

```text
Generic sorting algorithm
        ↓
knows how to sort
        ↓
uses comparison operators
        ↓
custom type defines what comparison means
```

For a `Rectangle`:

```text
Sorting algorithm
does not know about area

Rectangle
defines ordering by area
```

This allows the same sorting algorithm to work with many different types.

---

## Key Takeaways

- Templates allow generic functions and data structures.
- `T` is a placeholder for a type decided at compile time.
- A template type must support every operation used by the template.
- Custom types can define operators using operator overloading.
- `const T&` avoids copying while preventing modification.
- Comparison operators allow generic sorting algorithms to work with custom types.
- `<` defines ordering.
- `<=` can help merge sort preserve stability when equal elements are encountered.
- Stable sorting preserves the relative order of equal elements.
