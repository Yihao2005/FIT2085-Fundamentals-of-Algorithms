#include <iostream>
#include <string>
using namespace std;

double read_input(string prompt)
{
    int n;
    cout<< prompt << endl;
    cin >> n;
    return n;
}

void fill_array(double arr[], int n)
{
    cout<< "Please enter the elements of the array:" << endl;
    for (int i = 0; i < n; i++)
    {

        cin>>arr[i];
    }
}

void is_sorted(double arr[], int n)
{
    bool is_sorted = true;
    for (int i = 1; i < n; i++)
    {
        if (arr[i] < arr[i - 1])
        {
            is_sorted = false;
        }
    }
    if (is_sorted)
    {
        cout<<"Yes"<<endl;
    }else
    {
        cout<<"No"<<endl;
    }
}

int main()
{
    int n = read_input("Please enter the number of elements you want to sort");
    double arr[n];
    fill_array(arr, n);
    is_sorted(arr,n);
}