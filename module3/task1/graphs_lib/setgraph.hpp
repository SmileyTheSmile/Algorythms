#include <unordered_set>


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