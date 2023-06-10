#include <vector>
#include <cassert>
#include <iostream>
#include <unordered_set>
#include <algorithm> 


struct ListGraph {
public:
    ListGraph(int size, bool unoriented = false):
    _adjacencyLists(size + 1), _unoriented(unoriented) {}

    ~ListGraph() {}
    
    void AddEdge(int from, int to) {
        assert(isValidVertex(from) && isValidVertex(to));

        _adjacencyLists[from].push_back(to);
        if (_unoriented)
            _adjacencyLists[to].push_back(from);
    }

    int VerticesCount() const {
        return static_cast<int>(_adjacencyLists.size());
    }

    std::vector<int> GetNextVertices(int vertex) const {
        assert(isValidVertex(vertex));

        return _adjacencyLists[vertex];
    }
    
    std::vector<int> GetPrevVertices(int vertex) const {
        assert(isValidVertex(vertex));

        std::vector<int> prevVertices;
        for (int from = 0; from < _adjacencyLists.size(); from++)
            for (int to: _adjacencyLists[from])
                if (to == vertex)
                    prevVertices.push_back(from);
        return prevVertices;
    }

    bool IsHamiltonian(std::vector<int>& path) {
        if (path.size() + 1 != VerticesCount())
            return false;

        std::vector<bool> visited(VerticesCount(), false);
        visited[path[0]] = true;

        for (int i = 1; i < VerticesCount() - 1; i++) {
            int prev = path[i - 1];
            int curr = path[i];
            bool found = false;

            for (int j = 0; j < _adjacencyLists[prev].size(); j++) {
                if (_adjacencyLists[prev][j] == curr && !visited[curr]) {
                    visited[curr] = true;
                    found = true;
                    break;
                }
            }

            if (!found)
                return false;
        }

        return true;
    }
    
private:
    std::vector<std::vector<int>> _adjacencyLists;
    bool _unoriented;

    bool isValidVertex(int vertex) const {
        return 0 <= vertex && vertex < _adjacencyLists.size();
    }
};

int main() {
    int n = 0, m = 0, k = 0;
    std::cin >> n >> m >> k;

    ListGraph graph(n);

    int from = 0, to = 0;
    for (int i = 0; i < m; i++) {
        std::cin >> from >> to;
        graph.AddEdge(from, to);
    }

    std::vector<int> path(k);
    for (int i = 0; i < k; i++)
        std::cin >> path[i];
    
    std::cout << graph.IsHamiltonian(path)? 1: 0;

    return 0;
}