/*

Дан базовый интерфейс для представления ориентированного графа.
Необходимо написать несколько реализаций интерфейса:
    ListGraph, хранящий граф в виде массива списков смежности,
    MatrixGraph, хранящий граф в виде матрицы смежности,
    SetGraph, хранящий граф в виде массива хэш-таблиц/сбалансированных деревьев поиска,
    ArcGraph, хранящий граф в виде одного массива пар {from, to}.
Также необходимо реализовать конструктор, принимающий const IGraph&. 
Такой конструктор должен скопировать переданный граф в создаваемый объект.
Для каждого класса создавайте отдельные h и cpp файлы.
Число вершин графа задается в конструкторе каждой реализации.

*/


#include <iostream>
#include <queue>
#include <deque>
#include <functional>

#include "igraph.hpp"
#include "listgraph.hpp"
#include "setgraph.hpp"
#include "matrixgraph.hpp"
#include "arcgraph.hpp"

void BFS(const IGraph &graph, int vertex, std::vector<bool> &visited, std::function<void(int)> &func)
{
    std::queue<int> qu;
    qu.push(vertex);
    visited[vertex] = true;
    
    while (!qu.empty())
    {
        int currentVertex = qu.front();
        qu.pop();
        
        func(currentVertex);
        
        for (int nextVertex: graph.GetNextVertices(currentVertex))
        {
            if (!visited[nextVertex])
            {
                visited[nextVertex] = true;
                qu.push(nextVertex);
            }
        }
    }
}

void BFS(const IGraph &graph, std::function<void(int)> func)
{
    std::vector<bool> visited(graph.VerticesCount(), false);
    
    for (int i = 0; i < graph.VerticesCount(); i++)
    {
        if (!visited[i])
            BFS(graph, i, visited, func);
    }
}

void DFS(const IGraph &graph, int vertex, std::vector<bool> &visited, std::function<void(int)> &func)
{
    visited[vertex] = true;
    func(vertex);
    
    for (int nextVertex: graph.GetNextVertices(vertex))
    {
        if (!visited[nextVertex])
            DFS(graph, nextVertex, visited, func);
    }
}

void DFS(const IGraph &graph, std::function<void(int)> func)
{
    std::vector<bool> visited(graph.VerticesCount(), false);
    
    for (int i = 0; i < graph.VerticesCount(); i++)
    {
        if (!visited[i])
            DFS(graph, i, visited, func);
    }
}

void topologicalSort(const IGraph &graph, int vertex, std::vector<bool> &visited, std::deque<int> &sorted)
{
    visited[vertex] = true;
    
    for (int nextVertex: graph.GetNextVertices(vertex))
    {
        if (!visited[nextVertex])
            topologicalSort(graph, nextVertex, visited, sorted);
    }
    
    sorted.push_front(vertex);
}

std::deque<int> topologicalSort(const IGraph &graph)
{
    std::deque<int> sorted;
    std::vector<bool> visited(graph.VerticesCount(), false);
    
    for (int i = 0; i < graph.VerticesCount(); i++)
    {
        if (!visited[i])
            topologicalSort(graph, i, visited, sorted);
    }
    
    return sorted;
}


int main(int argc, const char *argv[]) {
    ArcGraph graph(9);
    graph.AddEdge(0, 1);
    graph.AddEdge(0, 5);
    graph.AddEdge(1, 2);
    graph.AddEdge(1, 3);
    graph.AddEdge(1, 5);
    graph.AddEdge(1, 6);
    graph.AddEdge(3, 2);
    graph.AddEdge(3, 4);
    graph.AddEdge(3, 6);
    graph.AddEdge(5, 4);
    graph.AddEdge(5, 6);
    graph.AddEdge(6, 4);
    graph.AddEdge(7, 8);

    /*
    int n = 0;
    std::cin >> n;

    ListGraph listGraph(n);
    for (size_t i = 0; i < n; i++) {
        int val1 = 0, val2 = 0;
        std::cin >> val1 >> val2;

        listGraph.AddEdge(val1, val2);
    }
    */
    BFS(graph, [](int vertex){ std::cout << vertex << " "; });
    std::cout << std::endl;
    DFS(graph, [](int vertex){ std::cout << vertex << " "; });
    std::cout << std::endl;

    for (int vertex: topologicalSort(graph)) {
        std::cout << vertex << " ";
    }
    std::cout << std::endl;
    
    return 0;
}
/*
0 1 5 2 3 6 4 7 8 
0 1 2 3 4 6 5 7 8 
7 8 0 1 5 3 6 4 2 
*/