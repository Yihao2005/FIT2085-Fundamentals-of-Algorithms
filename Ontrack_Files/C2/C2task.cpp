#include <iostream>
#include <string>

// ...
#include <codecvt>

#include "algorithms.h"
#include "data_structures.h"

using namespace std;

struct PostAnalyser {
    // ...
    LinkedList<Batch> batches;
    int batch_size;

    PostAnalyser(int n) {
        // ...
        batch_size = n;

    }

    void add_batch(Post* post_batch) {
        // ...
        merge_sort(post_batch, batch_size);
        Batch batch(post_batch, batch_size);
        batches.append(batch);
    }

    void remove_post(Post post) {
        // ...
        Node<Batch>* current_node = batches.first_node;
        while (current_node != nullptr)
        {
            int target_index = binary_search(current_node->item.posts, 0, current_node->item.size-1,post);
            if (target_index != -1)
            {
                current_node->item.remove_post(target_index);
                return;
            }
            current_node = current_node->next;

        }
    }

    Post* least_popular_over_threshold(int threshold)
    {
        // ...
        Post post;
        post.likes = threshold;
        post.post_name = "threshold";

        Node<Batch>* current_node = batches.first_node;
        Post* result = nullptr;

        while (current_node != nullptr)
        {
            int index = binary_search_threshold(current_node->item.posts, 0, current_node->item.size-1,post);

            if (index != -1)
            {
                if (result == nullptr || current_node->item.posts[index].likes < result->likes)
                {
                    result = &current_node->item.posts[index];
                }
            }
            current_node = current_node->next;
        }
        return result;
    }


};

int ontrack_test() {
    int n;
    cout << "Enter the batch size:" << endl;
    cin >> n;
    PostAnalyser analyser(n);

    string menu = "1. Add a batch\n2. Remove a post\n3. Find least popular post over a threshold\n4. See the menu again\n5. Quit";
    int command = 4;
    while (command != 5) {
        if (command == 1) {
            Post* batch_post = new Post[n];
            for (int i = 0; i < n; i++) {
                cout << "Enter likes for post " << i << ":" << endl;
                int likes;
                cin >> likes;

                cout << "Enter post name for post " << i << ":" << endl;
                string post_name;
                cin >> post_name;

                batch_post[i].likes = likes;
                batch_post[i].post_name = post_name;
            }
            analyser.add_batch(batch_post);
            delete[] batch_post;
        } else if (command == 2) {
            cout << "Enter likes of the post to remove:" << endl;
            int likes;
            cin >> likes;

            Post post;
            post.likes = likes;
            post.post_name = "To be deleted"; // The name doesn't matter for deletion, only likes are used for comparison
            analyser.remove_post(post);
        } else if (command == 3) {
            cout << "Enter threshold:" << endl;
            int threshold;
            cin >> threshold;
            Post* result = analyser.least_popular_over_threshold(threshold);
            if (result == nullptr) {
                cout << "No such post exists" << endl;
            } else {
                cout << "Post found with " << result->likes << " likes" << endl;
            }
        } else if (command == 4) {
            cout << menu << endl;
        }

        cout << "Enter command (1-5):" << endl;
        cin >> command;
        if (!cin) {
            cout << "ERROR: could not read next command" << endl;
            return 1;
        }
    }
    return 0;
}


int main(int argc, char** argv) {
    if (argc != 1) {
        return ontrack_test();
    }
    // Include any tests you want to write in code here.
    ontrack_test();


    return 0;
}