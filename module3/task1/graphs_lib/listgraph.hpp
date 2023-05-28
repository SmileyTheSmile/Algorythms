

struct ListGraph: public IGraph {
public:
    ListGraph(int size): _adjacencyLists(size) {}
    
    ListGraph(const IGraph &graph): _adjacencyLists(graph.VerticesCount()) {
        for (int i = 0; i < graph.VerticesCount(); i++)
            _adjacencyLists[i] = graph.GetNextVertices(i);
    }
    
    ~ListGraph() {}
    
    void AddEdge(int from, int to) override {
        assert(isValidVertex(from) && isValidVertex(to));

        _adjacencyLists[from].push_back(to);
    }

    int VerticesCount() const override {
        return static_cast<int>(_adjacencyLists.size());
    }

    std::vector<int> GetNextVertices(int vertex) const override {
        assert(0 <= vertex && vertex < _adjacencyLists.size());

        return _adjacencyLists[vertex];
    }
    
    std::vector<int> GetPrevVertices(int vertex) const override {
        assert(0 <= vertex && vertex < _adjacencyLists.size());

        std::vector<int> prevVertices;
        for (int from = 0; from < _adjacencyLists.size(); from++)
            for (int to: _adjacencyLists[from])
                if (to == vertex)
                    prevVertices.push_back(from);
        return prevVertices;
    }
    
private:
    std::vector<std::vector<int>> _adjacencyLists;

    bool isValidVertex(int vertex) const {
        return 0 <= vertex && vertex < _adjacencyLists.size();
    }
};