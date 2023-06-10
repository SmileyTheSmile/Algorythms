#include <iostream>
#include <algorithm>
#include <vector>
#include <cassert>
#include <set>
#include <limits>  


class ListGraph {
public:
    ListGraph(int size): _adjacencyLists(size) {}

    void AddEdge(int from, int to, int distance) {
        assert(isValidVertex(from) && isValidVertex(to));

        _adjacencyLists[from].push_back({distance, to});
        _adjacencyLists[to].push_back({distance, from});
    }


    int Dijkstra(int from, int to) {
        assert(isValidVertex(from) && isValidVertex(to));

        std::vector<int> distances(_adjacencyLists.size(), std::numeric_limits<int>::max());
        distances[from] = 0;
        
        std::multiset<std::pair<int, int>> dijkstraSet;
        dijkstraSet.insert({distances[from], from});

        while (!dijkstraSet.empty()) {
            int startVertex = dijkstraSet.begin()->second;
            dijkstraSet.erase(dijkstraSet.begin());

            for (std::pair<int, int> &vertex : _adjacencyLists[startVertex]) {
                int from = vertex.first, to = vertex.second;

                if (distances[to] <= distances[startVertex] + from)
                    continue;

                if (distances[startVertex] != std::numeric_limits<int>::max())
                    dijkstraSet.erase({distances[startVertex], startVertex});

                distances[to] = distances[startVertex] + from;
                dijkstraSet.insert({from, to});
            }
        }

        return distances[to];
    }

private:
    std::vector<std::vector<std::pair<int, int>>> _adjacencyLists;

    bool isValidVertex(int vertex) const {
        return 0 <= vertex && vertex < _adjacencyLists.size();
    }
};

int main() {
    int n = 0, m = 0;
    std::cin >> n >> m;

    ListGraph graph(n);
    
    int from = 0, to = 0, distance = 0;
    for (int i = 0; i < m; i++) {
        std::cin >> from >> to >> distance;
        graph.AddEdge(from, to, distance);
    }

    std::cin >> from >> to;
    std::cout << graph.Dijkstra(from, to) << "\n";
    return 0;
}