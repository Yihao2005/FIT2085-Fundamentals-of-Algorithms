#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

/// This function simulates giving us the number of available servers.
/// Ignore its implementation, all you need to know is that when you call
/// this function it returns an integer greater than 1. And you may assume it
/// does so in O(1) time.
/// No other assumptions can be made about this function.
int available_servers() {
    return 2 + rand() % 100;
}

/// `sublists` is the arrays that need to be merged
/// `sublist_sizes` is the size of each of those arrays (i.e. sublists[i] will have sublist_sizes[i] items)
/// `number_of_sublists` is the number of sublists passed to the function in the previous 2 parameters
int* merge_sorted_lists(int** sublists, int* sublist_sizes, int number_of_sublists) {
    // Implement the merging logic, return a pointer to the merged array on the heap

    // Calculate the total size of the merged list
    int total_size = 0;

    for (int i = 0; i < number_of_sublists; i++)
    {
        total_size += sublist_sizes[i];
    }

    // Allocate the merged list on the heap
    int* merged_list = new int[total_size];

    // Every sublists would have a index tracker
    int* indexes = new int[number_of_sublists];

    for (int i = 0; i < number_of_sublists; i++)
    {
        indexes[i] = 0;
    }

    // sorted_index is used to track the index =
    int sorted_index = 0;

    for (int i = 0; i < number_of_sublists; i++)
    {
        // First directly copy it
        if (sorted_index == 0)
        {

            for (int j = 0; j < sublist_sizes[i]; j++)
            {
                merged_list[sorted_index] = sublists[i][j];
                sorted_index++;
            }
            continue;

        }

        int temp_index = 0;
        int temp_size = sorted_index;
        int* temp_sorted_list = new int[sorted_index];

        // Copy the current merged list
        for (int j = 0; j < temp_size; j++)
        {
            temp_sorted_list[j] = merged_list[j];
        }

        // Rewrite the current merged list from index 0
        sorted_index = 0;

        while (temp_index < temp_size && indexes[i] < sublist_sizes[i])
        {
            if (temp_sorted_list[temp_index] < sublists[i][indexes[i]])
            {
                merged_list[sorted_index] = temp_sorted_list[temp_index];
                temp_index++;
            }else
            {
                merged_list[sorted_index] = sublists[i][indexes[i]];
                indexes[i]++;
            }
            sorted_index++;
        }

        while (temp_index < temp_size)
        {
            merged_list[sorted_index] = temp_sorted_list[temp_index];
            sorted_index++;
            temp_index++;
        }

        while (indexes[i] < sublist_sizes[i])
        {
            merged_list[sorted_index] = sublists[i][indexes[i]];
            sorted_index++;
            indexes[i]++;
        }
        delete[] temp_sorted_list;
    }

    delete[] indexes;

    return merged_list;
}

int* merge_sort(int* items, int n) {
    // Ask for the next k (the number of servers currently available)
    int k = available_servers();
    if (k > n) k = n;

    // Implement the sorting logic, return a pointer to the sorted array on the heap

    //base case
    if (n<=1)
    {
        int* result= new int[n];
        if (n==1)
        {
            result[0] = items[0];
        }
        return result;
    }

    //Split and copy the sublist
    int** sublists = new int*[k];
    int number_of_sublists = k;
    int* sublists_sizes = new int[number_of_sublists];

    int base_size = n/k;
    int reminder = n%k;

    for (int i = 0; i < number_of_sublists; i++)
    {
        sublists_sizes[i] = base_size;

        if (i <reminder)
        {
            sublists_sizes[i]++;
        }

        sublists[i] = new int[sublists_sizes[i]];
    }

    int temp_index = 0;

    for (int i = 0; i < k; i++)
    {
        for (int j = 0; j < sublists_sizes[i]; j++)
        {
            sublists[i][j] = items[temp_index];
            temp_index++;
        }
    }

    //
    for (int i = 0; i < k; i++)
    {
        int* sorted_sublist = merge_sort(sublists[i],sublists_sizes[i]);

        delete[] sublists[i];

        sublists[i] = sorted_sublist;

    }

    int* result = merge_sorted_lists(sublists, sublists_sizes, k);

    for (int i = 0; i < k; i++)
    {
        delete[] sublists[i];
    }

    delete[] sublists;
    delete[] sublists_sizes;

    return result;
}

int main(int argc, char** argv) {

    bool data_from_console = argc == 1; // run './a.out' and './a.out bottom text' to see the difference

    int* sorted;
    int n;

    if (data_from_console) {

        srand(10001);

        cout << "Enter number of data values:" << endl;
        cin >> n;
        int* data = new int[n];
        cout << "Enter data values:" << endl;
        for (int i = 0; i < n; i++) {
            cin >> data[i];
        }
        sorted = merge_sort(data, n);
        delete[] data;
    } else {
        srand(time(nullptr));
        n = 10;
        int data[] = {9, 4, 7, 2, 1, 6, 8, 3, 5, 0};

        sorted = merge_sort(data, n);
    }

    cout << "Sorted values:" << endl;
    for (int i = 0; i < n; i++) {
        cout << sorted[i] << " ";
        if ((i & 31) == 31) cout << '\n'; // Break up lines so that line by line diff is useful
    }
    cout << endl;

    return 0;
}
