//
// Created by Yihao Yu on 2/9/2026.
//

#ifndef FIT2085_ALGORITHMS_H
#define FIT2085_ALGORITHMS_H


template<typename T>
void merge_sort(T arr[], int n)
{
    if (n <= 1)
    {
        return;
    }

    int left_size = n / 2;
    int right_size = n - left_size;

    T* left_half = new T[left_size];
    T* right_half = new T[right_size];

    for (int i = 0; i < n; i++)
    {
        if (i < left_size)
        {
            left_half[i] = arr[i];
        }else
        {
            right_half[i-left_size] = arr[i];
        }
    }

    merge_sort(left_half, left_size);
    merge_sort(right_half, right_size);

    int left_half_index = 0;
    int right_half_index = 0;
    int result_index = 0;

    while (left_half_index < left_size && right_half_index < right_size)
    {
        if (left_half[left_half_index] < right_half[right_half_index])
        {
            arr[result_index] = left_half[left_half_index];
            left_half_index++;
        }else
        {
            arr[result_index] = right_half[right_half_index];
            right_half_index++;
        }
        result_index++;
    }

    while (left_half_index < left_size )
    {
        arr[result_index] = left_half[left_half_index];
        result_index++;
        left_half_index++;
    }

    while (right_half_index < right_size )
    {
        arr[result_index] = right_half[right_half_index];
        result_index++;
        right_half_index++;
    }

    delete[] left_half;
    delete[] right_half;
}

template<typename T>
int binary_search(T arr[], int left, int right, T target)
{
    if (left > right)
    {
        return -1;
    }

    int mid = (left+right)/2;

    if (arr[mid] == target)
    {
        return mid;
    }else if (arr[mid] > target)
    {
        return binary_search(arr, left, mid-1, target);
    }else
    {
        return binary_search(arr, mid+1, right, target);
    }
}

template<typename T>
int binary_search_threshold(T arr[], int left, int right, T target)
{
    if (left > right)
    {
        return -1;
    }

    int mid = (left+right)/2;

   if (arr[mid] > target)
   {
       if (mid == 0 || arr[mid-1] <= target )
       {
           return mid;
       }

       return binary_search_threshold(arr, left, mid-1, target);
   }

    return binary_search_threshold(arr, mid+1, right, target);
}

#endif //FIT2085_ALGORITHMS_H
