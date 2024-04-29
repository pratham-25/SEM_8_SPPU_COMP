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

                #pragma omp for
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

    void DFS(int start) {
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

    void parallelDFS(int start) {
        vector<bool> visited(getV(), false);

        DFSUtil(start, visited);
    }

    void createRandomGraph(int numEdges) {
        set<pair<int,int>> edgeSet;  // To avoid duplicate edges
        int numVertices = getV();

        for(int i = 0;i < numEdges;i++) {
            int u = rand() % numVertices;
            int v = rand() % numVertices;

            if(u != v && edgeSet.find({u,v}) == edgeSet.end() && edgeSet.find({v,u}) == edgeSet.end()) {
                cout << " {" << u << "," << v << "} ";
                edgeSet.insert({u,v});
                addEdge(u,v);
            }
        }
    }

    void printGraph() {
        for(int i = 0;i < getV();i++) {
            cout << i << " -> ";
            for(auto x : adj[i]) cout << x << " ";
            cout << "\n";
        }
    }

    int getV()  {
        return V;
    }

};

int main() {

    // Graph g(6);
    // int E;
    // cout << "Enter edges: ";
    // cin >> E;
    // for(int i = 0;i < E;i++) {
    //     cout << "Enter " << i+1 << " edge: ";
    //     int u, v;
    //     cin >> u >> v;
    //     g.addEdge(u,v);
    // }

    int numVertices, numEdges;
    cout << "Enter no. of vertices: ";
    cin >> numVertices;
    cout << "Enter no. of edges: ";
    cin >> numEdges;

    Graph g(numVertices);
    g.addEdge(0,1);
    g.createRandomGraph(numEdges);

    g.printGraph();

    auto start = chrono::high_resolution_clock::now();
    cout << "Sequential DFS:";
    g.DFS(0);
    cout << "\n";
    auto stop = chrono::high_resolution_clock::now();

    auto dfs_duration = chrono::duration_cast<chrono::microseconds>(stop-start);


    start = chrono::high_resolution_clock::now();
    cout << "Parallel DFS:";
    g.parallelDFS(0);
    cout << "\n";
    stop = chrono::high_resolution_clock::now();

    auto pdfs_duration = chrono::duration_cast<chrono::microseconds>(stop-start);


    start = chrono::high_resolution_clock::now();
    cout << "Sequential BFS:";
    g.BFS(0);
    cout << "\n";
    stop = chrono::high_resolution_clock::now();

    auto bfs_duration = chrono::duration_cast<chrono::microseconds>(stop-start);


    start = chrono::high_resolution_clock::now();
    cout << "Parallel BFS:";
    g.parallelBFS(0);
    cout << "\n";
    stop = chrono::high_resolution_clock::now();

    auto pbfs_duration = chrono::duration_cast<chrono::microseconds>(stop-start);

    cout << "Sequential DFS Time: " << dfs_duration.count() << " microseconds " << "\n";
    cout << "Parallel dFS Time:" << pdfs_duration.count() << " microseconds " << "\n";
    cout << "Sequential BFS Time: " << bfs_duration.count() << " microseconds " << "\n";
    cout << "Parallel BFS Time:" << pbfs_duration.count() << " microseconds " << "\n";


    return 0;
}

/*
-- Sample tc:
Enter edges: 6
Enter 1 edge: 0 1
Enter 2 edge: 0 2
Enter 3 edge: 0 3
Enter 4 edge: 1 4
Enter 5 edge: 1 5
Enter 6 edge: 2 5
Sequential DFS:0 1 4 5 2 3 
Parallel DFS:0 1 4 5 2 3 
Sequential BFS:0 1 2 3 4 5 
Parallel BFS:0 1 2 3 4 5 
Sequential DFS Time: 39 microseconds 
Parallel dFS Time:19 microseconds 
Sequential BFS Time: 24 microseconds 
Parallel BFS Time:23 microseconds
*/