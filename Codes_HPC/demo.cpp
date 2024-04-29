/**
#include<bits/stdc++.h>
#include<omp.h>
#include<chrono>
using namespace std;

class Graph {
    int V;
    vector<vector<int>> adj;
public:
    Graph(int V) {
        this->V = V;
        adj.resize(V);
    }

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void BFS(int start) {
        vector<bool> visited(getV(), false);
        queue<int> q;

        q.push(start);
        visited[start] = true;

        while(!q.empty()) {
            int x = q.front();
            q.pop();
            cout << x << " ";

            for(int i = 0;i < adj[x].size();i++) {
                if(!visited[adj[x][i]]) {
                    visited[adj[x][i]] = true;
                    q.push(adj[x][i]);
                }
            }
        }
    }

    void parallelBFS(int start) {
        vector<bool> visited(getV(), false);
        queue<int> q;

        q.push(start);
        visited[start] = true;

        while(!q.empty()) {
            int x;
            #pragma omp parallel shared(q,visited)
            {
            #pragma omp single
            {
            x = q.front();
            q.pop();
            cout << x << " ";
            }
            #pragma omp parallel for
            {
            for(int i = 0;i < adj[x].size();i++) {
                if(!visited[adj[x][i]]) {
                    #pragma omp critical
                    {
                    visited[adj[x][i]] = true;
                    q.push(adj[x][i]);
                    }
                    
                }
            }
            }
            }
            
        }
    }

    void DFSUtil(int x, vector<bool> &visited) {
        visited[x] = true;
        cout << x << " ";

        for(int i = 0;i < adj[x].size();i++) {
            if(!visited[adj[x][i]]) {
                DFSUtil(adj[x][i], visited);
            }
        }

    }

    void parallelDFS(int start) {
        vector<bool> visited(getV(), false);
        DFSUtil(start, visited);
    }

    void parallelDFSUtil(int x, vector<bool> &visited) {
        visited[x] = true;
        cout << x << " ";

        #pragma omp parallel for
        {
        for(int i = 0;i < adj[x].size();i++) {
        if(!visited[adj[x][i]]) {
            DFSUtil(adj[x][i], visited);
        }
        }
        }
    }

    void DFS(int start) {
        vector<bool> visited(getV(), false);
        DFSUtil(start, visited);
    }

    int getV() {
        return V;
    }

};


int main() {
    Graph g(6);
    int edges;
    cout << "Enter no. of edges: ";
    cin >> edges;

    for(int i = 0;i < edges;i++) {
        int u,v;
        cout << "Enter edge: ";
        cin >> u >> v;
        g.addEdge(u,v);
    }

    auto start = chrono::high_resolution_clock::now();
    g.BFS(0);
    auto end = chrono::high_resolution_clock::now();
    auto sbfs = chrono::duration_cast<chrono::microseconds>(end-start);
    cout << "Seq. BFS: " << sbfs.count() << " microseconds" << "\n";
    cout << "\n";
    g.DFS(0);
    cout << "\n";
    g.parallelBFS(0);
    cout << "\n";
    g.parallelDFS(0);
    cout << "\n";

    return 0;
}
*/

/*
#include<bits/stdc++.h>
#include<omp.h>
#include<chrono>
using namespace std;


void merge(vector<int> &arr, int low, int mid ,int high) {
    vector<int> tmp;
    int left = low;
    int right = mid + 1;

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

void parallelMergeSort(vector<int> &arr, int low, int high) {
    if(low >= high) return;

    int mid = (low + high)/2;

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

void mergeSort(vector<int> &arr, int low, int high) {
    if(low >= high) return;

    int mid = (low + high)/2;

    mergeSort(arr, low, mid);
    mergeSort(arr, mid+1, high);

    merge(arr, low, mid, high);
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


void bubbleSort(vector<int> &arr, int n) {
    for(int i = n-1;i >= 0;i--) {
        for(int j = 0;j <= i-1;j++) {
            if(arr[j] > arr[j+1]) swap(arr[j], arr[j+1]);
        }
    }
}

void reconstructArr(vector<int> &arr, vector<int> &copy, int n) {
    for(int i = 0;i < n;i++) {
        copy[i] = arr[i];
    }
}

void print(vector<int> &arr) {
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
    mergeSort(arr_copy, 0, n-1);
    print(arr_copy);
    auto end = chrono::high_resolution_clock::now();
    auto smerge = chrono::duration_cast<chrono::microseconds>(end-start);

    start = chrono::high_resolution_clock::now();
    cout << "Parallel Merge Sort: ";
    parallelMergeSort(arr_copy, 0, n-1);
    print(arr_copy);
    end = chrono::high_resolution_clock::now();
    auto pmerge = chrono::duration_cast<chrono::microseconds>(end-start);


    start = chrono::high_resolution_clock::now();
    cout << "Sequential Bubble Sort: ";
    bubbleSort(arr_copy,n);
    print(arr_copy);
    end = chrono::high_resolution_clock::now();
    auto sbubble = chrono::duration_cast<chrono::microseconds>(end-start);


    start = chrono::high_resolution_clock::now();
    cout << "Parallel Bubble Sort: ";
    parallelBubbleSort(arr_copy, n);
    print(arr_copy);
    end = chrono::high_resolution_clock::now();
    auto pbubble = chrono::duration_cast<chrono::microseconds>(end-start);

    cout << "Sequential Merge Sort: " << smerge.count() << " microseconds" << "\n";
    cout << "Parallel Merge Sort: " << pmerge.count() << " microseconds" << "\n";
    cout << "Sequential Bubble Sort: " << sbubble.count() << " microseconds" << "\n";
    cout << "Parallel Bubble Sort: " << pbubble.count() << " microseconds" << "\n";

    return 0;
}

*/
