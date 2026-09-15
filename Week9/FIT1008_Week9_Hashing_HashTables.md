# FIT1008 / FIT1054 / FIT2085 — Week 9 Notes
## Strings, Hashing, Hash Tables, and Separate Chaining

---

# 1. More on Strings

## 1.1 Everything is Stored as Numbers

Computers ultimately store information as binary values. Characters are therefore represented internally by numbers.

One common mapping is **ASCII**.

Examples:

```text
'a' -> 97
'b' -> 98
'A' -> 65
'0' -> 48
'1' -> 49
```

In C++:

```cpp
int x = 'a';
cout << x;   // 97
```

The opposite conversion also works:

```cpp
char c = 97;
cout << c;   // a
```

---

## 1.2 Character vs String

A character uses single quotes:

```cpp
char c = 'a';
```

A string uses double quotes:

```cpp
string s = "a";
```

They are different types.

A character is represented by one numeric code.

A string is a sequence of characters.

Example:

```cpp
string s = "hello";
```

Conceptually:

```text
'h' 'e' 'l' 'l' 'o'
104 101 108 108 111
```

You can access individual characters using indexing:

```cpp
string s = "hello";

cout << s[0];      // h

int ascii = s[0];
cout << ascii;     // 104
```

---

# 2. Converting Between Character Digits and Integers

The character:

```cpp
'7'
```

does **not** have the integer value `7`.

Its ASCII value is:

```text
'7' -> 55
```

To convert a digit character into its numeric value:

```cpp
int digit = c - '0';
```

Example:

```cpp
char c = '7';

int digit = c - '0';
```

Because:

```text
'7' - '0'
= 55 - 48
= 7
```

The reverse conversion is:

```cpp
char c = '0' + digit;
```

So remember:

```text
character digit -> integer digit
c - '0'

integer digit -> character digit
'0' + digit
```

---

# 3. Exercise: `str_to_int`

The goal is to convert:

```text
"1234"
```

into:

```text
1234
```

The main idea is:

```text
result = result * 10 + new_digit
```

Example:

```text
0
0 * 10 + 1 = 1
1 * 10 + 2 = 12
12 * 10 + 3 = 123
123 * 10 + 4 = 1234
```

Implementation:

```cpp
int str_to_int(string s)
{
    int result = 0;

    for (int i = 0; i < s.length(); i++)
    {
        int digit = s[i] - '0';

        result = result * 10 + digit;
    }

    return result;
}
```

---

# 4. Exercise: `int_to_str`

The opposite operation converts:

```text
1234
```

into:

```text
"1234"
```

Use `% 10` to extract the final digit:

```text
1234 % 10 = 4
```

Then remove the final digit using integer division:

```text
1234 / 10 = 123
```

The digits are extracted backwards:

```text
4, 3, 2, 1
```

Therefore each new digit can be inserted at the beginning of the string.

Example:

```cpp
string int_to_str(int x)
{
    if (x == 0)
    {
        return "0";
    }

    string result = "";

    while (x > 0)
    {
        int digit = x % 10;

        char c = '0' + digit;

        result = c + result;

        x /= 10;
    }

    return result;
}
```

---

# 5. Hashing

Hashing converts an input into a smaller **fingerprint**.

Conceptually:

```text
large input
    |
    v
hash function
    |
    v
small hash value
```

Example:

```text
"hello"
    |
    v
hash()
    |
    v
3827162
```

A function that generates hash values is called a:

```text
Hash Function
```

Hashing is useful when working with large data because comparing short fingerprints may be much cheaper than comparing the original objects directly.

---

# 6. Polynomial Rolling Hash

A simple hash can process characters one at a time.

The basic pattern is:

```cpp
hash_value = hash_value * base + ascii_code;
```

This is very similar to `str_to_int()`:

```cpp
result = result * 10 + digit;
```

For `"hello"`:

```text
h = 104
e = 101
l = 108
l = 108
o = 111
```

Using base `b`, the resulting expression is:

```text
h*b^4 + e*b^3 + l*b^2 + l*b + o
```

This is why it is called a **Polynomial Rolling Hash**.

---

# 7. Why Character Position Matters

If the hash were only the sum of ASCII values:

```text
hash("abc")
=
97 + 98 + 99
```

and:

```text
hash("cba")
=
99 + 98 + 97
```

the two hashes would be identical.

Using position-dependent weights solves this problem:

```text
a*b^2 + b*b + c
```

is normally different from:

```text
c*b^2 + b*b + a
```

Therefore the location of each character influences the final hash.

---

# 8. Collisions

Two different inputs can sometimes produce the same hash value.

Example:

```text
hash("abc") = 12345

hash("xyz") = 12345
```

This is called a:

```text
collision
```

Collisions cannot be completely eliminated when the number of possible inputs is larger than the number of possible hash outputs.

For example, if the hash output is always between:

```text
0 and 9,999,999
```

then only a finite number of hash values are possible.

However, there are infinitely many possible strings.

Therefore collisions must theoretically exist.

A good hash function tries to:

- minimise collisions;
- distribute values evenly;
- avoid obvious patterns.

---

# 9. Why a Small Base Is Bad

Suppose the base is `10`.

ASCII character values can be much larger than `9`.

For example:

```text
"an"

10 * 97 + 110
= 1080
```

and:

```text
"bd"

10 * 98 + 100
= 1080
```

This produces an immediate collision.

The problem is that the base is too small relative to possible character values.

Therefore we want:

```text
base > maximum character value
```

The Week 9 implementation uses:

```cpp
const int base = 271;
```

---

# 10. Why Use Modulo?

Without modulo, repeated multiplication causes the hash value to become extremely large.

Eventually it may overflow.

To keep the result bounded:

```cpp
hash_value %= mod;
```

For example:

```cpp
const int mod = 10000019;
```

ensures:

```text
0 <= hash_value < 10000019
```

We can apply modulo after every iteration because modular arithmetic preserves the final result.

Useful identities:

```text
(a + b) mod m
=
((a mod m) + (b mod m)) mod m
```

and:

```text
(a * b) mod m
=
((a mod m) * b) mod m
```

---

# 11. Improved Polynomial Rolling Hash

```cpp
int hash(string text)
{
    const int base = 271;
    const int mod = 10000019;

    long long hash_value = 0;

    for (int i = 0; i < text.length(); i++)
    {
        int ascii_code = text[i];

        hash_value =
            hash_value * base + ascii_code;

        hash_value %= mod;
    }

    return hash_value;
}
```

---

# 12. Hash Function Time Complexity

If the string has length `n`, every character is processed exactly once.

Therefore:

```text
Time Complexity = O(n)
```

If the application guarantees a fixed upper bound on key length, then the hashing cost can be treated as:

```text
O(1)
```

For example, if every key is an email address with a bounded maximum length.

---

# 13. Hash Tables

A hash table uses a hash function to map keys to array indices.

Conceptually:

```text
key
 |
 v
hash function
 |
 v
array index
 |
 v
value
```

Example:

```text
"abc@student.monash.edu"
            |
            v
          hash()
            |
            v
          482917
            |
            v
      table[482917]
```

---

# 14. Why Hash Tables Are Fast

Array indexing is:

```text
O(1)
```

For example:

```cpp
table[482917]
```

can be accessed directly.

Instead of searching through every item, the hash function tells us which table position to inspect.

This transforms:

```text
search using complicated key
```

into:

```text
direct array access
```

---

# 15. Key-Value Pairs

Hash tables normally store:

```text
key -> value
```

Examples:

```text
email -> Account
student ID -> Student
word -> frequency
username -> profile
```

The key is the attribute used to locate the corresponding value.

---

# 16. Choosing Keys

Keys should normally be unique.

Good keys:

```text
email address
student ID
username
employee ID
```

Poor keys:

```text
name
age
city
```

For example, several people may have the same name, so a name cannot reliably identify exactly one account.

---

# 17. Basic Hash Table

A simple implementation might look like:

```cpp
struct HashTable
{
    const int table_size = 10000019;

    string* table = new string[table_size];

    int hash(string key)
    {
        const int base = 271;

        long long hash_value = 0;

        for (int i = 0; i < key.length(); i++)
        {
            int ascii_code = key[i];

            hash_value =
                hash_value * base + ascii_code;

            hash_value %= table_size;
        }

        return hash_value;
    }

    void insert(string key, string value)
    {
        int position = hash(key);

        table[position] = value;
    }

    string search(string key)
    {
        int position = hash(key);

        return table[position];
    }
};
```

The same key always produces the same hash value, so the value can later be retrieved from the same location.

---

# 18. The Main Problem: Collisions in Hash Tables

Suppose:

```text
hash("abc@email.com") = 1874
hash("xyz@email.com") = 1874
```

Then:

```cpp
table[1874] = "Alice";
```

followed by:

```cpp
table[1874] = "Bob";
```

would overwrite Alice.

We therefore need a collision-resolution strategy.

---

# 19. Separate Chaining

The Week 9 solution is:

```text
Separate Chaining
```

Instead of storing one item at each table index, store a linked list of all items that hash to that index.

Example:

```text
table

0 -> nullptr

1 -> [key/value] -> nullptr

2 -> nullptr

3 -> [key/value]
      |
      v
     [key/value]
      |
      v
     [key/value]
      |
      v
     nullptr
```

Each table index is called a **bucket**.

Each bucket contains the start of a chain.

---

# 20. Separate Chaining Node

```cpp
struct SPNode
{
    SPNode* next = nullptr;

    string key;

    string value;
};
```

Each node stores:

```text
key
value
next pointer
```

This is essentially a linked-list node.

---

# 21. Why `SPNode** table`?

The table is an array of pointers to nodes.

Therefore:

```cpp
SPNode** table =
    new SPNode*[table_size];
```

Conceptually:

```text
table
 |
 v
array of SPNode*
```

Each element:

```cpp
table[i]
```

has type:

```cpp
SPNode*
```

and points to the first node of the chain.

Example:

```text
table[0] -> Node -> Node -> nullptr
table[1] -> nullptr
table[2] -> Node -> nullptr
```

---

# 22. Initialising the Table

The pointer array must initially contain `nullptr`.

```cpp
HashTable()
{
    for (int i = 0; i < table_size; i++)
    {
        table[i] = nullptr;
    }
}
```

Without initialisation, the pointers could contain garbage addresses.

`nullptr` clearly means:

```text
this bucket is empty
```

---

# 23. Inserting into a Hash Table with Separate Chaining

The insertion algorithm is:

1. Hash the key.
2. Find the correct bucket.
3. Search the chain for the key.
4. If the key already exists, update its value.
5. Otherwise, insert a new node.

Example:

```cpp
void insert(string key, string value)
{
    int table_position = hash(key);

    SPNode* current_node =
        table[table_position];

    while (current_node != nullptr)
    {
        if (current_node->key == key)
        {
            current_node->value = value;
            return;
        }

        current_node =
            current_node->next;
    }

    SPNode* node = new SPNode;

    node->key = key;
    node->value = value;

    node->next =
        table[table_position];

    table[table_position] = node;
}
```

---

# 24. Why Insert at the Beginning?

Suppose a chain is:

```text
A -> B -> C
```

To insert `X` at the beginning:

```cpp
node->next = head;
head = node;
```

Result:

```text
X -> A -> B -> C
```

This insertion itself is:

```text
O(1)
```

Inserting at the end would require traversal unless a tail pointer were maintained.

---

# 25. Searching

Search works by:

1. hashing the key;
2. jumping directly to the correct bucket;
3. searching only that chain.

```cpp
string search(string key)
{
    int position = hash(key);

    SPNode* current_node =
        table[position];

    while (current_node != nullptr)
    {
        if (current_node->key == key)
        {
            return current_node->value;
        }

        current_node =
            current_node->next;
    }

    throw string("Key not found!");
}
```

The key insight is:

```text
Hash Table does NOT normally search every item.
```

Instead:

```text
hash key
   |
   v
jump to bucket
   |
   v
search short chain
```

---

# 26. Deletion

Deleting from a linked chain requires both:

```text
current node
previous node
```

Example:

```text
A -> B -> C
```

To delete `B`:

```text
A.next = C
```

Implementation:

```cpp
void del(string key)
{
    int position = hash(key);

    SPNode* current_node =
        table[position];

    SPNode* prev_node = nullptr;

    while (current_node != nullptr)
    {
        if (current_node->key == key)
        {
            if (prev_node == nullptr)
            {
                table[position] =
                    current_node->next;
            }
            else
            {
                prev_node->next =
                    current_node->next;
            }

            delete current_node;

            return;
        }

        prev_node = current_node;
        current_node =
            current_node->next;
    }

    throw string("Key not found!");
}
```

---

# 27. Why the First Node Is a Special Case

Suppose:

```text
table[5]
   |
   v
  [A]
   |
   v
  [B]
   |
   v
  [C]
```

If `A` is deleted, there is no previous node.

Therefore:

```cpp
table[5] = current_node->next;
```

After deletion:

```text
table[5]
   |
   v
  [B]
   |
   v
  [C]
```

This is why the code checks:

```cpp
if (prev_node == nullptr)
```

---

# 28. Hash Table Time Complexity

Let:

```text
c = key length
n = number of items stored
```

Hashing costs:

```text
O(c)
```

If key length is bounded:

```text
O(1)
```

---

# 29. Best Case

If the correct bucket is empty or the required item is immediately found:

```text
hashing = O(1)
chain operation = O(1)
```

Therefore:

```text
Best-case insertion/search/deletion = O(1)
```

assuming key length is bounded.

If key length is not treated as constant:

```text
O(c)
```

---

# 30. Worst Case

In the worst case, every key hashes to the same bucket.

The hash table becomes effectively one large linked list:

```text
table[5]
   |
   v
node
 |
 v
node
 |
 v
node
 |
 v
...
```

Searching the chain may require visiting all `n` elements.

Therefore:

```text
Worst case = O(c + n)
```

If key length is bounded:

```text
Worst case = O(n)
```

Important:

```text
Hash Table is NOT guaranteed O(1) in the worst case.
```

Its common advantage is:

```text
average / expected O(1)
```

with a good hash function and appropriate table size.

---

# 31. Load Factor

Let:

```text
n = number of stored items
m = number of buckets
```

The load factor is:

```text
alpha = n / m
```

If values are distributed evenly, the average chain length is approximately:

```text
n / m
```

If the load factor remains bounded by a small constant, average search cost is also approximately constant.

Therefore:

```text
Average Hash Table operation
≈ O(1)
```

---

# 32. Hash Table vs Balanced BST

| Operation | Balanced BST | Hash Table Average | Hash Table Worst |
|---|---:|---:|---:|
| Search | O(log n) | O(1) | O(n) |
| Insert | O(log n) | O(1) | O(n) |
| Delete | O(log n) | O(1) | O(n) |

Hash tables are especially good for:

```text
exact-key lookup
```

Balanced BSTs are especially useful for:

```text
ordered operations
range queries
minimum / maximum
predecessor / successor
sorted traversal
```

A hash table loses the natural ordering of keys because hashing distributes them across apparently unrelated indices.

---

# 33. Getting All Keys

Suppose the hash table provides:

```cpp
string* keys();
```

To return every key, the program must:

1. iterate through every bucket;
2. traverse every chain;
3. collect every key.

Conceptually:

```cpp
for each bucket
{
    current = table[i];

    while (current != nullptr)
    {
        collect current->key;

        current =
            current->next;
    }
}
```

If:

```text
m = number of buckets
n = number of stored items
```

then:

```text
Time Complexity = O(m + n)
```

because all buckets and all stored nodes may need to be inspected.

---

# 34. Word Frequency Application

Hash tables are often used as frequency maps.

Input:

```text
hello world hello WORLD
```

Desired output:

```text
hello: 2
world: 1
WORLD: 1
```

Store:

```text
word -> count
```

Processing:

```text
read "hello"
-> not present
-> insert hello: 1

read "world"
-> not present
-> insert world: 1

read "hello"
-> already present
-> increment to 2

read "WORLD"
-> different key from "world"
-> insert WORLD: 1
```

The structure becomes:

```text
hello -> 2
world -> 1
WORLD -> 1
```

---

# 35. Week 9 Conceptual Flow

The entire week can be understood as one sequence:

```text
Character
   |
   v
ASCII integer
   |
   v
String = sequence of characters
   |
   v
Convert string information into numbers
   |
   v
Polynomial Rolling Hash
   |
   v
String key -> integer hash
   |
   v
integer hash -> array index
   |
   v
Hash Table
   |
   v
Direct bucket access
   |
   v
Collisions occur
   |
   v
Separate Chaining
   |
   v
Array of linked lists
```

---

# 36. Core Mental Model

```text
                  HASH TABLE

key
 |
 v
+---------------+
| Hash Function |
+---------------+
 |
 v
index
 |
 v

table

[0] -> nullptr

[1] -> [key/value]
        |
        v
       nullptr

[2] -> nullptr

[3] -> [key/value]
        |
        v
       [key/value]
        |
        v
       [key/value]
        |
        v
       nullptr
```

The **array** provides fast bucket access.

The **linked lists** handle collisions.

Together they form a hash table using **Separate Chaining**.

---

# 37. Key Facts to Remember

- Characters are represented numerically.
- ASCII maps characters to integer codes.
- `'7'` and `7` are different values.
- Use `c - '0'` to convert a digit character to an integer.
- Use `'0' + digit` to convert an integer digit to a character.
- Polynomial rolling hash processes characters one by one.
- Hash functions can produce collisions.
- Collisions cannot theoretically be eliminated for arbitrary strings and finite hash ranges.
- Hash tables map keys to array indices using hash functions.
- Separate chaining stores colliding items in linked lists.
- Hash tables normally provide average `O(1)` insertion, search, and deletion.
- Worst-case complexity can degrade to `O(n)`.
- Hash tables are best for exact-key lookup.
- Balanced BSTs are better when ordering or range queries are required.

---

# 38. Important Complexity Summary

| Operation | Complexity |
|---|---:|
| Hash a string of length `c` | O(c) |
| Hash bounded-length key | O(1) |
| Hash table search, best/average | O(1) |
| Hash table insert, best/average | O(1) |
| Hash table delete, best/average | O(1) |
| Hash table search, worst | O(n) |
| Hash table insert, worst | O(n) |
| Hash table delete, worst | O(n) |
| Return all keys | O(m + n) |

Where:

```text
c = key length
n = number of stored items
m = number of buckets
```

---

# 39. Week 9 Revision Checklist

You should be able to explain:

- What ASCII is.
- The difference between a character and a string.
- How to implement `str_to_int`.
- How to implement `int_to_str`.
- What hashing means.
- What a hash function does.
- Why polynomial rolling hash uses different positional weights.
- What a collision is.
- Why collisions cannot be completely avoided.
- Why the base should be sufficiently large.
- Why modulo is used.
- Why modulo can be applied after every iteration.
- How a hash table maps keys to array indices.
- What key-value pairs are.
- Why keys should be unique.
- Why simple hash tables fail when collisions occur.
- How separate chaining solves collisions.
- Why `SPNode**` is used.
- How hash-table insertion works.
- How hash-table search works.
- How hash-table deletion works.
- Why deletion of the first node is a special case.
- Best-, average-, and worst-case complexities.
- The difference between hash tables and balanced BSTs.
- How a hash table can be used for word-frequency counting.
