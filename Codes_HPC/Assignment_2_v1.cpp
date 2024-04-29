#include<bits/stdc++.h>
#include<omp.h>
#include<chrono>
using namespace std;


void bubbleSort(vector<int> &arr, int n) {
    for(int i = n-1;i >= 0;i--) {
        for(int j = 0;j <= i-1;j++) {
            if(arr[j] > arr[j+1]) swap(arr[j], arr[j+1]);
        }
    }
}

void parallelBubbleSort(vector<int> &arr, int n) {
    for(int i = n-1;i >= 0;i--) {
        #pragma omp parallel for
        {
        for(int j = 0;j <= i-1;j++) {
            if(arr[j] > arr[j+1]) swap(arr[j], arr[j+1]);
        }
        }
    }
}

void merge(vector<int> &arr, int low, int mid, int high) {
    vector<int> tmp;
    int left = low;
    int right = mid+1;

    while(left <= mid && right <= high) {
        if(arr[left] <= arr[right]) {
            tmp.push_back(arr[left]);
            left++;
        }
        else {
            tmp.push_back(arr[right]);
            right++;
        }
    }

    while(left <= mid) {
        tmp.push_back(arr[left]);
        left++;
    }
    while(right <= high) {
        tmp.push_back(arr[right]);
        right++;
    }

    for(int i = low;i <= high;i++) {
        arr[i] = tmp[i - low];
    }
}

void mergeSort(vector<int> &arr, int low, int high) {
    if(low >= high) return;

    int mid = (low + high) / 2;
    mergeSort(arr, low, mid);
    mergeSort(arr, mid+1, high);

    merge(arr, low, mid, high); 
}

void parallelMergeSort(vector<int> &arr, int low, int high) {
    if(low >= high) return;

    int mid = (low + high) / 2;

    #pragma omp parallel sections
    {
        #pragma omp section
        {
            parallelMergeSort(arr, low, mid);
        }
        #pragma omp section
        {
            parallelMergeSort(arr, mid+1, high);
        }
    }

    merge(arr, low, mid, high); 
}

void createArr(vector<int> &arr_copy, vector<int> &arr, int n) {
    for(int i = 0;i < n;i++) {
        arr_copy[i] = arr[i];
    }
}

void printArr(vector<int> &arr){
    for(auto x : arr) cout << x << " ";
    cout << "\n";
}

int main() {

    int n;
    cout << "Enter no. of elements: ";
    cin >> n;

    vector<int> arr(n), arr_copy(n);

    for(int i = 0;i < n;i++) {
        arr[i] = rand() % 1000;
        arr_copy[i] = arr[i];
    }

    auto start = chrono::high_resolution_clock::now();
    cout << "Sequential Merge Sort: ";
    createArr(arr_copy,arr,n);
    mergeSort(arr_copy, 0, n-1);
    printArr(arr_copy);
    auto end = chrono::high_resolution_clock::now();
    auto smerge = chrono::duration_cast<chrono::microseconds>(end-start);

    start = chrono::high_resolution_clock::now();
    cout << "Parallel Merge Sort: ";
    createArr(arr_copy,arr,n);
    parallelMergeSort(arr_copy, 0, n-1);
    printArr(arr_copy);
    end = chrono::high_resolution_clock::now();
    auto pmerge = chrono::duration_cast<chrono::microseconds>(end-start);

    start = chrono::high_resolution_clock::now();
    cout << "Sequential Bubble Sort: ";
    createArr(arr_copy,arr,n);
    bubbleSort(arr_copy, n-1);
    printArr(arr_copy);
    end = chrono::high_resolution_clock::now();
    auto sbubble = chrono::duration_cast<chrono::microseconds>(end-start);

    start = chrono::high_resolution_clock::now();
    cout << "Parallel Bubble Sort: ";
    createArr(arr_copy,arr,n);
    parallelBubbleSort(arr_copy, n-1);
    printArr(arr_copy);
    end = chrono::high_resolution_clock::now();
    auto pbubble = chrono::duration_cast<chrono::microseconds>(end-start);


    cout << "Sequential Merge Sort: " << smerge.count() << " microseconds" << "\n";
    cout << "Parallel Merge Sort: " << pmerge.count() << " microseconds" << "\n";
    cout << "Sequential Bubble Sort: " << sbubble.count() << " microseconds" << "\n";
    cout << "Parallel Bubble Sort: " << pbubble.count() << " microseconds" << "\n";

    return 0;
}