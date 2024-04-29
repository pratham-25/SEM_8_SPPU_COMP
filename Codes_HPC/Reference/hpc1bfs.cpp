#include <iostream>
#include <queue>
#include <vector>
#include <omp.h>

using namespace std;

const int MAXN = 100005; // maximum number of vertices

vector<int> adj[MAXN]; // adjacency list representation
int level[MAXN]; // array to store the level of each vertex
bool visited[MAXN]; // array to keep track of visited vertices

void bfs(int s) {
    queue<int> q;
    q.push(s);
    level[s] = 0;
    visited[s] = true;

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        #pragma omp parallel for
        for (int i = 0; i < adj[u].size(); i++) {
            int v = adj[u][i];
            if (!visited[v]) {
                visited[v] = true;
                level[v] = level[u] + 1;
                q.push(v);
            }
        }
    }
}

int main() {
    int n, m, s; // number of vertices, number of edges, source vertex
    cin >> n >> m >> s;

    for (int i = 0; i < m; i++) {
        int u, v; // edge from u to v
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    bfs(s);

    // print out the level of each vertex
    for (int i = 1; i <= n; i++) {
        cout << "Level of vertex " << i << " is " << level[i] << endl;
    }

    return 0;
}
