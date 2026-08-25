#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

struct Rectangle {
    // ...
    int width;
    int height;

    bool operator<(const Rectangle& right) const
    {
        if (width*height < right.width*right.height)
        {
            return true;
        }else
        {
            return false;
        }
    }

    bool operator<=(const Rectangle& right) const
    {
        if (width*height <= right.width*right.height)
        {
            return true;
        }else
        {
            return false;
        }
    }
};

template <typename T>
void insertion_sort(T arr[], int size ) {
    // ...
    for (int first_unsorted_index = 0; first_unsorted_index < size; first_unsorted_index++)
    {
        int current_index = first_unsorted_index;
        T temp = arr[current_index];
        while (current_index -1 >= 0 && temp <= arr[current_index - 1])
        {
            arr[current_index] = arr[current_index - 1];
            current_index--;
        }

        arr[current_index] = temp;

    }

}

template <typename T>
void merge_sort(T arr[], int n) {
    // ...
    if (n<=1) return;

    int left_have_size = n / 2;
    int right_have_size = n - left_have_size;

    T* left_half = new T[left_have_size];
    T* right_half = new T[right_have_size];

    for (int i = 0; i < n; i++)
    {
        if (i < left_have_size)
        {
            left_half[i] = arr[i];
        }else
        {
            right_half[i-left_have_size] = arr[i];
        }
    }

    merge_sort(left_half, left_have_size);
    merge_sort(right_half, right_have_size);

    int left_half_index = 0;
    int right_half_index = 0;
    int result_index = 0;

    while (left_half_index < left_have_size && right_half_index < right_have_size)
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

    while (left_half_index < left_have_size)
    {
        arr[result_index] = left_half[left_half_index];
        left_half_index++;
        result_index++;
    }

    while (right_half_index < right_have_size)
    {
        arr[result_index] = right_half[right_half_index];
        right_half_index++;
        result_index++;
    }

    delete[] left_half;
    delete[] right_half;
}

int main() {
    srand(time(nullptr));
    Rectangle rects_1[10], rects_2[10];

    for (int i = 0; i < 10; i++) {
        rects_1[i].width = rand() % 20;
        rects_1[i].height = rand() % 20;
        rects_2[i].width = rects_1[i].width;
        rects_2[i].height = rects_1[i].height;
    }

    insertion_sort(rects_1, 10);
    merge_sort(rects_2, 10);
    int last_area = -1;

    for (int i = 0; i < 10; i++) {
        int cur_area = rects_1[i].width * rects_1[i].height;
        if (cur_area < last_area) {
            cout << "The sorted arrays don't match!" << endl;
            return 1;
        }
        last_area = cur_area;
    }

    for (int i = 0; i < 10; i++) {
        if (rects_1[i].width != rects_2[i].width || rects_1[i].height != rects_2[i].height) {
            cout << "The sorted arrays don't match!" << endl;
            return 1;
        }
    }

    cout << "Everything seems fine : )" << endl;
    return 0;
}


