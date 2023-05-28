#include <utility>


struct ArcGraph : public IGraph {
public:
    explicit ArcGraph(int vertexCount): _vertexCount(vertexCount) {}

    ArcGraph(const IGraph& other) : _pairs() {
        for (int from = 0; from < other.VerticesCount(); from++)
            for (int to : other.GetNextVertices(from))
                _pairs.push_back(std::make_pair(from, to));
    }

    void AddEdge(int from, int to) override {
        assert(isValidVertex(from) && isValidVertex(to));
        
        _pairs.push_back(std::make_pair(from, to));
    }

    int VerticesCount() const override {
        return _vertexCount;
    }

    std::vector<int> GetNextVertices(int vertex) const override {
        assert(isValidVertex(vertex));

        std::vector<int> nextVertices;
        for (const auto& path : _pairs) {
            int from = path.first, to = path.second;
            if (from == vertex)
                nextVertices.push_back(to);
        }
        return nextVertices;
    }

    std::vector<int> GetPrevVertices(int vertex) const override {
        assert(isValidVertex(vertex));

        std::vector<int> prevVertices;
        for (const auto& path : _pairs) {
            int from = path.first, to = path.second;
            if (to == vertex)
                prevVertices.push_back(from);
        }
        return prevVertices;
    }

private:
    std::vector<std::pair<int, int>> _pairs;
    int _vertexCount;

    bool isValidVertex(int vertex) const {
        return 0 <= vertex && vertex < _vertexCount;
    }
};