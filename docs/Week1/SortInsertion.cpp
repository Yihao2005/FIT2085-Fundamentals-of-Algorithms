#include <iostream>
using namespace std;

void insertion_sort(int arr[], int n)
{
    for (int first_unsorted_index = 0; first_unsorted_index < n; first_unsorted_index++)
    {
        int current_index = first_unsorted_index;

        while (current_index - 1 >= 0 && arr[current_index - 1] > arr[current_index])
        {
            swap(arr[current_index - 1], arr[current_index]);
            current_index--;
        }

    }
}

int main()
{
    int arr[10] = {1,2,4,3,5,9,7,8,6,10};
    insertion_sort(arr, 10);
    cout<< "New array items: " << endl;
    for (int i = 0; i < 10; i++)
    {
        cout << arr[i] << " ";
    }
    cout << endl;
}