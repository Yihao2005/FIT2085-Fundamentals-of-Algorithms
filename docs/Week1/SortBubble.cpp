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