#include <unordered_set>


struct SetGraph : public IGraph {
public:
    SetGraph(int size, bool unoriented = false):
    _adjacencySet(size), _unoriented(unoriented) {}

    SetGraph(const IGraph& other): _adjacencySet(other.VerticesCount()) {
        for (int from = 0; from < other.VerticesCount(); from++)
            for (int to : other.GetNextVertices(from))
                _adjacencySet[from].insert(to);
    }

    void AddEdge(int from, int to) override {
        assert(isValidVertex(from) && isValidVertex(to));

        _adjacencySet[from].insert(to);
        if (_unoriented)
            _adjacencySet[to].insert(from);
    }

    int VerticesCount() const override {
        return _adjacencySet.size();
    }

    std::vector<int> GetNextVertices(int vertex) const override {
        assert(isValidVertex(vertex));

        std::vector<int> nextVertices;
        for (int nextVertex : _adjacencySet[vertex])
            nextVertices.push_back(nextVertex);
        return nextVertices;
    }

    std::vector<int> GetPrevVertices(int vertex) const override {
        assert(isValidVertex(vertex));

        std::vector<int> prevVertices;
        for (int from = 0; from < _adjacencySet.size(); from++)
            if (_adjacencySet[from].count(vertex) > 0)
                prevVertices.push_back(from);
        return prevVertices;
    }

private:
    std::vector<std::unordered_set<int>> _adjacencySet;
    bool _unoriented;

    bool isValidVertex(int vertex) const {
        return 0 <= vertex && vertex < _adjacencySet.size();
    }
};