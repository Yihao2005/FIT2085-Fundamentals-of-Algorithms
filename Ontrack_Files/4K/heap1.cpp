#include <iostream>
#include <string>

using namespace std;

struct MinHeap
{
    int* array = new int[1];
    int size = 0;

    int current_capacity = 0;

    void resize()
    {
        current_capacity = (current_capacity + 1) * 2;
        int* new_array = new int[current_capacity+1];

        for (int i = 1; i <= size; i++)
        {
            new_array[i] = array[i];
        }

        delete[] array;
        array = new_array;
    }

    int parent(int index)
    {
        return index / 2;
    }

    int left_child (int node_index)
    {
        return 2 * node_index;
    }

    int right_child (int node_index)
    {
        return 2 * node_index + 1;
    }

    void add(int value)
    {
        if (size == current_capacity)
        {
            resize();
        }

        array[size+1] = value;
        size++;

        int current_size = size;

        while (parent(current_size) >=1 && array[parent(current_size)] > array[current_size])
        {
            swap(array[parent(current_size)], array[current_size]);
            current_size = parent(current_size);
        }
    }

    void sink(int node_index)
    {
        while (left_child(node_index) <= size)
        {
            int small_child_index = left_child(node_index);

            if (right_child(node_index) <= size)
            {
                if (array[left_child(node_index)] > array[right_child(node_index)])
                {
                    small_child_index = right_child(node_index);
                }
            }

            if (array[small_child_index] < array[node_index])
            {
                swap(array[small_child_index], array[node_index]);
                node_index = small_child_index;
            } else
            {
                break;
            }
        }
    }

    int extract_min()
    {
        if (size == 0)
        {
            throw "The heap is empty";
        }

        int min = array[1];
        swap(array[1], array[size]);
        size--;

        sink(1);
        return min;
    }

    void heapify(int arr[], int n)
    {
        delete [] array;
        array = new int[n+1];
        current_capacity = n;

        for (int i = 0; i < n; i++)
        {
            array[i+1] = arr[i];
        }
        size = n;

        for (int i = n; i >= 1; i--)
        {
            sink(i);
        }
    }
};

/*
 * We process the questions in their original order.
 * The min heap holds the scores of the currently selected questions.
 * Total is the sum of those scores.
 *
 * The heap is used to find the minimum selected score, and it
 * doesn't change the order in which the retained questions are attempted.
 *
 * Before processing a new question, every prefix of the retained sequence has a non negative value.
 * Adding the new question only introduces one prefix: the whole retained sequence
 * ending at that question.
 *
 * If the sum is negative, the new score must be negative.
 * Let the S be the total before adding new score (S >= 0),
 * X be the new score,
 * M be the minimum score in the MinHeap.
 * Since M<=X, S + X < 0,
 * therefore, S + X - M = S + ( X - M ) >= 0.
 *
 * M is negative. Removing it leaves prefix sums before its
 * position unchanged and increases those after its position.
 * Therefore, every retained prefix remains non-negative.
 *
 * Above all, we could prove that one removal is sufficient.
 *
 * When the total becomes negative, we cannot keep all currently
 * selected questions, so a removal is necessary.
 * Removing M restores validity with one removal and gives the
 * largest remaining total among all single-removal choices.
 * This leaves the most room for future negative scores.
 */
int main()
{
    MinHeap heap;
    int n;
    cin >> n; // the total number of questions on test
    long long total = 0;
    for (int i = 0; i < n; i++)
    {
        long long val;
        cin >> val; // the expected mark of ith question

        // temporarily keep the current question
        total += val;
        heap.add(val);

        if (total < 0)
        {
            // Remove the smallest selected score to achieve
            // largest increment in total from removing one question
            long long min = heap.extract_min();
            total -= min;
        }
    }

    cout << heap.size << endl;
    return 0;
}
