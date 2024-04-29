#include<bits/stdc++.h>
#include<omp.h>
#include<chrono>
using namespace std;

int Min(vector<int> &arr) {
    int res = arr[0];
    for(auto x : arr) {
        res = min(res, x);
    }

    return res;
}

int parallelMin(vector<int> &arr) {
    int res = arr[0];

    #pragma omp parallel for reduction(min : res)
    {
        for(auto x : arr) {
            res = min(res, x);
        }
    }

    return res;
}

int Max(vector<int> &arr) {
    int res = arr[0];
    for(auto x : arr) {
        res = max(res, x);
    }

    return res;
}

int parallelMax(vector<int> &arr) {
    int res = arr[0];

    #pragma omp parallel for reduction(max : res)
    {
        for(auto x : arr) {
            res = max(res, x);
        }
    } 

    return res;
}


int Sum(vector<int> &arr) {
    int res = 0;
    for(auto x : arr) {
        res += x;
    }

    return res;
}

int parallelSum(vector<int> &arr) {
    int res = 0;

    #pragma omp parallel for reduction(+ : res)
    {
        for(auto x : arr) {
            res += x;
        }
    }

    return res;
}

double Avg(vector<int> &arr) {
    int sum = Sum(arr);
    return (double)(sum)/arr.size();
}

double parallelAvg(vector<int> &arr) {
    int sum = Sum(arr);
    return (double)(sum)/arr.size();
}

int main() {

    int n;
    cout << "Enter number of entries: ";
    cin >> n;

    vector<int> arr(n);

    for(int i = 0;i < n;i++) {
        arr[i] = rand() % 1000;
    }

    auto start = chrono::high_resolution_clock::now();
    cout << "The minimum value is: " << Min(arr) << '\n';
    auto end = chrono::high_resolution_clock::now();
    auto smin = chrono::duration_cast<chrono::microseconds>(end-start); 

    start = chrono::high_resolution_clock::now();
    cout << "The parallel minumum value is: " << parallelMin(arr) << "\n";
    end = chrono::high_resolution_clock::now();
    auto pmin = chrono::duration_cast<chrono::microseconds>(end-start); 

    start = chrono::high_resolution_clock::now();
    cout << "The maximum value is: " << Max(arr) << '\n';
    end = chrono::high_resolution_clock::now();
    auto smax = chrono::duration_cast<chrono::microseconds>(end-start); 

    start = chrono::high_resolution_clock::now();
    cout << "The parallel maximum value is: " << parallelMax(arr) << "\n";
    end = chrono::high_resolution_clock::now();
    auto pmax = chrono::duration_cast<chrono::microseconds>(end-start); 

    start = chrono::high_resolution_clock::now();
    cout << "The sum is: " << Sum(arr) << '\n';
    end = chrono::high_resolution_clock::now();
    auto ssum = chrono::duration_cast<chrono::microseconds>(end-start); 

    start = chrono::high_resolution_clock::now();
    cout << "The parallel sum is: " << parallelSum(arr) << "\n";
    end = chrono::high_resolution_clock::now();
    auto psum = chrono::duration_cast<chrono::microseconds>(end-start); 

    start = chrono::high_resolution_clock::now();
    cout << "The average is: " << Avg(arr) << '\n';
    end = chrono::high_resolution_clock::now();
    auto savg = chrono::duration_cast<chrono::microseconds>(end-start); 

    start = chrono::high_resolution_clock::now();
    cout << "The parallel average is: " << parallelAvg(arr) << "\n";
    end = chrono::high_resolution_clock::now();
    auto pavg = chrono::duration_cast<chrono::microseconds>(end-start); 


    cout << "Sequential Min: " << smin.count() << " microseconds\n";
    cout << "Parallel Min: " << pmin.count() << " microseconds\n";
    cout << "Sequential Max: " << smax.count() << " microseconds\n";
    cout << "Parallel Max: " << pmax.count() << " microseconds\n";
    cout << "Sequential Sum: " << ssum.count() << " microseconds\n";
    cout << "Parallel Sum: " << psum.count() << " microseconds\n";
    cout << "Sequential Avg: " << savg.count() << " microseconds\n";
    cout << "Parallel Avg: " << pavg.count() << " microseconds\n";

    return 0;
}