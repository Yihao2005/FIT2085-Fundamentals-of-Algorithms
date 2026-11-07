# Week 1
## 1. Sorting | Selection Sort

> 网站补充：排序原理与复杂度。以下 C++ 示例保留原笔记。

每一轮从未排序区间选出最小值，与区间首项交换，再将已排序区间向右扩展一格。例如 `[3, 1, 2] → [1, 3, 2] → [1, 2, 3]`。

- 最好、平均和最坏时间复杂度均为 **O(n²)**，额外空间 **O(1)**。
- 通常实现不稳定；远距离交换可能改变相同键值元素的相对顺序。


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

> 网站补充：排序原理与复杂度。以下 C++ 示例保留原笔记。

维护左侧已排序区间，每次取出下一个元素，向左比较并移动到合适的位置。下面的实现使用相邻交换。例如 `[3, 1, 2] → [1, 3, 2] → [1, 2, 3]`。

- 已有序时为 **O(n)**；平均和最坏为 **O(n²)**，额外空间 **O(1)**。
- 下面使用严格大于比较，相等元素不会交换，因此是稳定排序。

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

> 网站补充：排序原理与复杂度。以下 C++ 示例保留原笔记。

反复比较相邻元素，若顺序错误就交换；每一轮把当前较大元素向右推。下面的实现如果一整轮没有交换，就提前结束。

- 带提前结束优化时，最好为 **O(n)**；平均和最坏为 **O(n²)**，额外空间 **O(1)**。
- 只在严格大于时交换，因此是稳定排序。

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