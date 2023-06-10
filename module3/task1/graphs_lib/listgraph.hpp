

struct ListGraph: public IGraph {
public:
    ListGraph(int size, bool unoriented = false):
    _adjacencyLists(size), _unoriented(unoriented) {}
    
    ListGraph(const IGraph &graph): _adjacencyLists(graph.VerticesCount()) {
        for (int i = 0; i < graph.VerticesCount(); i++)
            _adjacencyLists[i] = graph.GetNextVertices(i);
    }
    
    ~ListGraph() {}
    
    void AddEdge(int from, int to) override {
        assert(isValidVertex(from) && isValidVertex(to));

        _adjacencyLists[from].push_back(to);
        if (_unoriented)
            _adjacencyLists[to].push_back(from);
    }

    int VerticesCount() const override {
        return static_cast<int>(_adjacencyLists.size());
    }

    std::vector<int> GetNextVertices(int vertex) const override {
        assert(isValidVertex(vertex));

        return _adjacencyLists[vertex];
    }
    
    std::vector<int> GetPrevVertices(int vertex) const override {
        assert(isValidVertex(vertex));

        std::vector<int> prevVertices;
        for (int from = 0; from < _adjacencyLists.size(); from++)
            for (int to: _adjacencyLists[from])
                if (to == vertex)
                    prevVertices.push_back(from);
        return prevVertices;
    }
    
private:
    std::vector<std::vector<int>> _adjacencyLists;
    bool _unoriented;

    bool isValidVertex(int vertex) const {
        return 0 <= vertex && vertex < _adjacencyLists.size();
    }
};