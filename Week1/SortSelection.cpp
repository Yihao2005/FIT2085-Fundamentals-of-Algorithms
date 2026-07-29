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

