#include <iostream>
#include <string>

using namespace std;

struct Post {
    string username;
    string type; // will be either "photo" or "video"
    int likes;
    int comments;
    int length_in_seconds; // only applies if it's a video
    int posted_timestamp; // the timestamp of when it was posted
};

int calculate_cost(int mem_write_cost, int mem_read_cost, int mem_store_cost, int write_count, int read_count, int store_count)
{
    return mem_write_cost*write_count + mem_read_cost*read_count + mem_store_cost*store_count;
}

// Selection Sort
int biggest_number_index(Post posts[], int n, int from_index)
{
    int current_biggest_index = from_index;
    for (int i = from_index+1; i<n; i++)
    {
        if (posts[i].likes > posts[current_biggest_index].likes)
        {
            current_biggest_index = i;
        }
        else if (posts[i].likes == posts[current_biggest_index].likes and posts[i].posted_timestamp < posts[current_biggest_index].posted_timestamp)
        {
            current_biggest_index = i;
        }
    }
    return current_biggest_index;
}

void selection_sort(Post posts[], int n)
{
    for (int i = 0; i < n; i++)
    {
        int next_biggest_index = biggest_number_index(posts, n, i);
        swap(posts[i],posts[next_biggest_index]);
    }
}

int estimate_selection_cost(int n, int mem_write_cost, int mem_read_cost, int mem_store_cost)
{
    int read_count = 2*(n-1) + n * (n - 1);
    int write_count = 2*(n-1);
    int store_count = 0;

    return calculate_cost(mem_write_cost, mem_read_cost, mem_store_cost, write_count, read_count, store_count);
}

// Insertion Sort
void insertion_sort(Post posts[], int n)
{
    for (int first_unsorted_index = 0; first_unsorted_index < n; first_unsorted_index++)
    {
        int current_index = first_unsorted_index;
        while (current_index - 1 >=0 &&
            (posts[current_index].likes > posts[current_index - 1].likes
             || (posts[current_index - 1].likes == posts[current_index].likes && posts[current_index - 1].posted_timestamp > posts[current_index].posted_timestamp)   ))
        {
            swap(posts[current_index],posts[current_index - 1]);
            current_index --;
        }
    }
}

int estimate_insertion_cost(int n, int mem_write_cost, int mem_read_cost, int mem_store_cost)
{
    int read_count = 2* n * (n-1);
    int write_count = n * (n-1);
    int store_count = 0;

    return calculate_cost(mem_write_cost, mem_read_cost, mem_store_cost, write_count, read_count, store_count);
}

// Bubble Sort
void bubble_sort(Post posts[], int n)
{
    for (int pass_counter = 0; pass_counter < n; pass_counter++)
    {
        bool pass_had_swaped = false;

        for (int left_item=0; left_item < n-1; left_item++)
        {
            if ((posts[left_item].likes < posts[left_item+1].likes)
                || (posts[left_item].likes == posts[left_item+1].likes && posts[left_item].posted_timestamp > posts[left_item+1].posted_timestamp))
            {
                swap(posts[left_item],posts[left_item+1]);
                pass_had_swaped = true;
            }
        }

        if (!pass_had_swaped) break;
    }
}

int estimate_bubble_cost(int n, int mem_write_cost, int mem_read_cost, int mem_store_cost)
{
    int read_count = 2*n*(n-1) + n * (n - 1);
    int write_count = n*(n-1);
    int store_count = 0;

    return calculate_cost(mem_write_cost, mem_read_cost, mem_store_cost, write_count, read_count, store_count);
}

// Cost comparison
int minimize_cost(Post posts[], int n, int mem_write_cost, int mem_read_cost, int mem_store_cost)
{
    int selection_cost = estimate_selection_cost(n, mem_write_cost, mem_read_cost, mem_store_cost);
    int insertion_cost = estimate_insertion_cost(n, mem_write_cost, mem_read_cost, mem_store_cost);
    int bubble_cost = estimate_bubble_cost(n, mem_write_cost, mem_read_cost, mem_store_cost);
    cout << "The cost for Selection sort is: " << selection_cost << endl;
    cout << "The cost for Insertion sort is: " << insertion_cost << endl;
    cout << "The cost for Bubble sort is: " << bubble_cost << endl;
    cout << "So I would choose: ";

    if (selection_cost <= insertion_cost && selection_cost <= bubble_cost)
    {
        selection_sort(posts,n);
        cout << "Selection sort"<<endl;
        return selection_cost;
    }
    else if (insertion_cost <= selection_cost && insertion_cost <= bubble_cost)
    {
        insertion_sort(posts,n);
        cout << "Insertion sort"<<endl;
        return insertion_cost;
    }
    else if (bubble_cost <= selection_cost && bubble_cost <= insertion_cost)
    {
        bubble_sort(posts,n);
        cout << "Bubble sort"<<endl;
        return bubble_cost;
    }
}


int sort_posts(Post posts[], int n, int mem_write_cost, int mem_read_cost, int mem_store_cost) {
    // ... implement your logic here ...
    // Return the estimated cost
    return minimize_cost(posts, n, mem_write_cost, mem_read_cost, mem_store_cost);
}

int main() {
    Post posts[10];
    for (int i = 0; i < 10; i++) {
        posts[i].username = "Cool user";
        posts[i].type = "photo";
        posts[i].likes = i;
        posts[i].comments = 3 * i;
        posts[i].length_in_seconds = 0;
        posts[i].posted_timestamp = i;
    }

    int cost = sort_posts(posts, 10, 1, 1, 1);

    cout << "Cost of sorting was " << cost << endl;
    cout << "# likes are:" << endl;
    for (int i = 0; i < 10; i++) {
        cout << posts[i].likes << " ";
    }
    cout << endl;
}