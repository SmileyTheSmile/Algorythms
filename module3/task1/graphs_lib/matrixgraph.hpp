#include <algorithm>


// Изначально я сделал это из массива массивов, но потом передумал и переделал под вектор векторов.
struct MatrixGraph: public IGraph {
public:
    MatrixGraph(const int arraySize):
    _adjacencyMatrix(arraySize, std::vector<bool>(arraySize, false)) { }    
    
    MatrixGraph(const IGraph &graph) {
        for (int i = 0; i < graph.VerticesCount(); i++)
            for (int vertex : graph.GetNextVertices(i))
                _adjacencyMatrix[i][vertex] = true;
    }
    
    ~MatrixGraph() {}   
    
    void AddEdge(int from, int to) override {
        assert(isValidVertex(from) && isValidVertex(to));

        _adjacencyMatrix[from][to] = true;
    }

    int VerticesCount() const override {
        return static_cast<int>(_adjacencyMatrix.size());
    }

    std::vector<int> GetNextVertices(int vertex) const override {
        assert(isValidVertex(vertex));
        
        std::vector<int> nextVertices;
        for (size_t i = 0; i < _adjacencyMatrix.size(); i++)
            if (_adjacencyMatrix[vertex][i])
                nextVertices.push_back(_adjacencyMatrix[vertex][i]);
        return nextVertices;
    }
    
    std::vector<int> GetPrevVertices(int vertex) const override {
        assert(isValidVertex(vertex));

        std::vector<int> prevVertices;
        for (int i = 0; i < _adjacencyMatrix.size(); i++)
            if (_adjacencyMatrix[i][vertex])
                prevVertices.push_back(i);
        return prevVertices;
    }
    
private:
    std::vector<std::vector<bool>> _adjacencyMatrix;

    bool isValidVertex(int vertex) const {
        return 0 <= vertex && vertex < _adjacencyMatrix.size();
    }
};