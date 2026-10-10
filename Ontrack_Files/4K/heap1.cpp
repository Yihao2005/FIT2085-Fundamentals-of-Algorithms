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

int main()
{
    MinHeap heap;
    int n;
    cin >> n;
    long long total = 0;
    for (int i = 0; i < n; i++)
    {
        long long val;
        cin >> val;
        total += val;
        heap.add(val);
        if (total < 0)
        {
            long long min = heap.extract_min();
            total -= min;
        }
    }

    cout << heap.size << endl;
    return 0;
}
