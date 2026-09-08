#include <iostream>
#include <string>

// ...
#include <codecvt>

#include "algorithms.h"
#include "data_structures.h"

using namespace std;

struct PostAnalyser {
    // ...
    LinkedList<Batch<Post>> batches;
    int batch_size;

    PostAnalyser(int n) {
        // ...
        batch_size = n;

    }

    void add_batch(Post* post_batch) {
        // ...
        merge_sort(post_batch, batch_size);
        Batch<Post> batch(post_batch, batch_size);
        batches.append(batch);
    }

    void remove_post(Post post) {
        // ...
        Node<Batch<Post>>* current_node = batches.first_node;
        while (current_node != nullptr)
        {
            current_node->item.delete_item(post);
            current_node = current_node->next;

        }
    }

    Post* least_popular_over_threshold(int threshold)
    {
        // ...
       Node<Batch<Post>>* current_node = batches.first_node;

        Post* result = nullptr;
        Post threshold_post;
        threshold_post.likes = threshold;
        threshold_post.post_name = "Threshold";


        while (current_node != nullptr)
        {
            Post* post = current_node->item.least_over_threshold(threshold_post);

            if (post != nullptr)
            {
               if ( result == nullptr || post->likes < result->likes)
               {
                   result = post;
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