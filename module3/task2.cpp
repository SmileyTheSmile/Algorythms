#include <unordered_set>
#include <vector>
#include <cassert>
#include <iostream>


struct IGraph {
    virtual ~IGraph() {}
        
    virtual void AddEdge(int from, int to) = 0;

    virtual int VerticesCount() const  = 0;

    virtual std::vector<int> GetNextVertices(int vertex) const = 0;
    virtual std::vector<int> GetPrevVertices(int vertex) const = 0;
};


struct SetGraph : public IGraph {
public:
    explicit SetGraph(int vertexCount): adjacencySet(vertexCount) {}

    SetGraph(const IGraph& other): adjacencySet(other.VerticesCount()) {
        for (int from = 0; from < other.VerticesCount(); from++)
            for (int to : other.GetNextVertices(from))
                adjacencySet[from].insert(to);
    }

    void AddEdge(int from, int to) override {
        if (isValidVertex(from) && isValidVertex(to))
            adjacencySet[from].insert(to);
    }

    int VerticesCount() const override {
        return adjacencySet.size();
    }

    std::vector<int> GetNextVertices(int vertex) const override {
        assert(isValidVertex(vertex));

        std::vector<int> nextVertices;
        for (int nextVertex : adjacencySet[vertex])
            nextVertices.push_back(nextVertex);
        return nextVertices;
    }

    std::vector<int> GetPrevVertices(int vertex) const override {
        assert(isValidVertex(vertex));

        std::vector<int> prevVertices;
        for (int from = 0; from < adjacencySet.size(); from++)
            if (adjacencySet[from].count(vertex) > 0)
                prevVertices.push_back(from);
        return prevVertices;
    }

private:
    std::vector<std::unordered_set<int>> adjacencySet;

    bool isValidVertex(int vertex) const {
        return 0 <= vertex && vertex < adjacencySet.size();
    }
};


int main(int argc, const char *argv[]) {
    int n = 0;
    std::cin >> n;

    SetGraph listGraph(n);
    for (size_t i = 0; i < n; i++) {
        int from = 0, to = 0, time = 0;
        std::cin >> from >> to >> time;

        listGraph.AddEdge(from, to);
    }
    
    return 0;
}