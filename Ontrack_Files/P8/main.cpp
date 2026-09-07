#include <iostream>
#include <string>
#include <cmath>

#include "structures.h"

using namespace std;

// One BST and one Max Heap already created for you in the global scope.
// These can be used in all of the functions below.
BinarySearchTree bst;
MaxHeap heap;

void add_post(Post* post) {
    // ...
    heap.add(post);
}

Node* search_node(Node* node, string username)
{
    if (node == nullptr || node->key == username)
    {
        return node;
    }

    if (username < node->key)
    {
        return search_node(node->left, username);
    }

    return search_node(node->right, username);
}

void print_trending(int k) {
    // ...
    // This is the print format expected for each trending post.
    // Feel free to use it in your code when needed:
    // cout << "Trending post by " << username << " with " << likes << " likes." << endl;
    for (int i = 0; i < k; i++)
    {
        Post* post = heap.extract_max();

        if (post == nullptr)
        {
            return;
        }

        cout << "Trending post by " << post->username << " with " << post->likes << " likes." << endl;

        Post* current_best = bst.search(post->username);

        if (current_best == nullptr)
        {
            bst.insert(post->username, post);
        }
        else if (current_best->likes < post->likes)
        {
            Node* node = search_node(bst.root, post->username);
            node->post = post;
        }
    }
}

Post* best_trending_of_user(string username) {
    // ...
    return bst.search(username);
}

int get_height (Node* node)
{
    if (node == nullptr)
    {
        return -1;
    }

    int left_height = get_height(node->left);
    int right_height = get_height(node->right);

    return 1 + max(left_height, right_height);
}

bool is_system_efficient(BinarySearchTree* system_bst) {
    // ...
    int n = system_bst->size;

    if (n == 0)
    {
        return true;
    }

    int h = get_height(system_bst->root);

    return pow(2,h) <= 5*n;

}

void test_is_system_efficient() {
    /* SAMPLE TEST */
    // Create a new BST
    BinarySearchTree test_bst;
    // Add a bunch of keys that will form a line, to make sure the tree is very unbalanced
    test_bst.insert("a", nullptr);
    test_bst.insert("b", nullptr);
    test_bst.insert("c", nullptr);
    test_bst.insert("d", nullptr);
    test_bst.insert("e", nullptr);
    test_bst.insert("f", nullptr);

    // Call the function, see what it says for this BST
    bool function_response = is_system_efficient(&test_bst);
    // If it says it's not efficient, then the test has passed.
    if (!function_response) cout << "Test 1 for system_efficient PASSED." << endl;
    // Otherwise, if the code thinks this tree is efficient, it has failed.
    else cout << "Test 1 for system_efficient FAILED." << endl;

    /* YOUR TESTS */
    // ...
    BinarySearchTree balanced_bst;
    balanced_bst.insert("d", nullptr);
    balanced_bst.insert("b", nullptr);
    balanced_bst.insert("f", nullptr);
    balanced_bst.insert("a", nullptr);
    balanced_bst.insert("c", nullptr);
    balanced_bst.insert("e", nullptr);
    balanced_bst.insert("g", nullptr);

    bool function_response_2 = is_system_efficient(&balanced_bst);
    if (function_response_2 ) cout << "Test 2 for system_efficient PASSED." << endl;
    else cout << "Test 2 for system_efficient FAILED." << endl;

    BinarySearchTree unbalanced_bst;
    unbalanced_bst.insert("g", nullptr);
    unbalanced_bst.insert("f", nullptr);
    unbalanced_bst.insert("e", nullptr);
    unbalanced_bst.insert("d", nullptr);
    unbalanced_bst.insert("c", nullptr);
    unbalanced_bst.insert("b", nullptr);
    unbalanced_bst.insert("a", nullptr);

    bool function_response_3 = is_system_efficient(&unbalanced_bst);
    if (!function_response_3) cout << "Test 3 for system_efficient PASSED." << endl;
    else cout << "Test 3 for system_efficient FAILED." << endl;
}

int ontrack_tests() {
    string menu = "1. Add a post\n2. Get the next trending list\n3. Check user's trending post\n4. Test if a tree is efficient\n5. See the menu again\n6. Quit";

    cout << menu << endl;
    int command = 0;
    cout << "Enter command (1-6):" << endl;
    cin >> command;

    while (command != 6) {
        if (command == 1) {
            cout << "Enter the username of the poster:" << endl;
            string username;
            cin >> username;

            cout << "Enter the number of likes:" << endl;
            int likes;
            cin >> likes;

            Post* post = new Post;
            post->username = username;
            post->posted_at = time(0);
            post->likes = likes;
            post->comments = 0;

            add_post(post);

        } else if (command == 2) {
            cout << "Enter how many posts to read from the trending list:" << endl;
            int k;
            cin >> k;

            print_trending(k);
        } else if (command == 3) {
            cout << "Enter the username:" << endl;
            string username;
            cin >> username;

            Post* post = best_trending_of_user(username);
            if (post == nullptr) {
                cout << "User does not have any trending posts" << endl;
            } else {
                cout << "User's best trending post has " << post->likes << " likes." << endl;
            }
        } else if (command == 4) {
            int n;
            cout << "Enter number of nodes" << endl;
            cin >> n;
            BinarySearchTree temp = BinarySearchTree();
            cout << "Enter node keys:" << endl;
            for (int i = 0; i < n; i++) {
                string key;
                cin >> key;
                temp.insert(key, nullptr);
            }
            if (is_system_efficient(&temp)) {
                cout << "Tree is efficient" << endl;
            } else {
                cout << "Tree is not efficient" << endl;
            }
        } else if (command == 5) {
            cout << menu << endl;
        } else {
            cout << "Unknown command." << endl;
        }

        cout << "Enter command (1-6):" << endl;
        cin >> command;
        if (!cin) {
            cerr << "ERROR: could not read next command" << endl;
            return 1;
        }
    }

    return 0;
}

int main(int argc, char** argv) {

    if (argc == 2) {
        return ontrack_tests();
    }

    // Once the commands above are done, the tests for system efficiency will run here.
    test_is_system_efficient();

    return 0;
}