#include <iostream>
#include <vector>
#include <stack>
#include <omp.h>

using namespace std;

void dfs(vector<vector<int>>& graph, int start) {
    vector<bool> visited(graph.size(), false);
    stack<int> st;
    st.push(start);

    #pragma omp parallel
    
        while (!st.empty()) {
            int cur = st.top();
            st.pop();

            if (!visited[cur]) {
                #pragma omp critical
                
                    if (!visited[cur]) {
                        visited[cur] = true;
                        cout << cur << " ";
                    }
                

                #pragma omp for schedule(dynamic)
                for (int i = 0; i < graph[cur].size(); ++i) {
                    int neighbor = graph[cur][i];
                    if (!visited[neighbor]) {
                        st.push(neighbor);
                    }
                }
            }
        }
    
}

int main() {
    vector<vector<int>> graph = {
        {1, 2},
        {0, 3},
        {0, 3},
        {1, 2, 4},
        {3}
    };
    int start = 0;

    dfs(graph, start);

    return 0;
}
