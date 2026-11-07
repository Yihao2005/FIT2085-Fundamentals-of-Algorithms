# Week 1
## 1. Sorting | Selection Sort

```C++
#include <iostream> 

using namespace std;

int smallest_number_index(int arr[], int n, int from_index){
    int current_smallest_index = from_index;

    for (int i = from_index+1 ; i < n ; i++){
        if (arr[i] < arr[current_smallest_index]){
            current_smallest_index = i;
        }
    }

    return current_smallest_index;
}

void selection_sort(int arr[], int n){
    for (int i = 0; i < n; i++){
        int next_smallest_index = smallest_number_index(arr, n, i);
        swap(arr[i],arr[next_smallest_index]);
    }
}

int main(){
    int arr[10] = {8,2,3,4,5,1,6,9,7};
    selection_sort(arr, 9);
    cout<< "New array items: "<< endl;
    for (int i = 0; i<9; i++){
        cout<<arr[i]<<" ";
    }
    cout<<endl;
}
```

## 2. Sorting | Insertion Sort
```C++
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
```

## 3. Sorting | Bubble Sort
```C++
#include <iostream>
using namespace std;

void bubble_sort(int arr[], int n)
{
    for (int pass_counter = 0; pass_counter < n; pass_counter++)
    {
        bool pass_had_swaped = false;

        for (int left_item = 0; left_item < n - 1; left_item++)
        {
            if (arr[left_item] > arr[left_item + 1])
            {
                swap(arr[left_item], arr[left_item + 1]);
                pass_had_swaped = true;
            }
        }

        if (!pass_had_swaped) break;
    }
}

int main()
{
    int arr[10] = {1,3,2,4,6,5,8,9,7,10};
    bubble_sort(arr,10);
    cout<<"Sorted array is: ";
    for (int i = 0; i < 10; i++)
    {
        cout<<arr[i]<<" ";
    }
    cout<<endl;
    return 0;
}
```