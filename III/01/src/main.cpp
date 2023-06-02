#include <iostream>

#include <functional>
#include <queue>
#include <vector>

#include "ArcGraph.hpp"
#include "IGraph.hpp"
#include "ListGraph.hpp"
#include "MatrixGraph.hpp"
#include "SetGraph.hpp"

/*
Дан базовый интерфейс для представления ориентированного графа:
struct IGraph {
  virtual ~IGraph() {}
	
	// Добавление ребра от from к to.
  virtual void AddEdge(int from, int to) = 0;

	virtual int VerticesCount() const  = 0;

  virtual std::vector<int> GetNextVertices(int vertex) const = 0;
  virtual std::vector<int> GetPrevVertices(int vertex) const = 0;
};

Необходимо написать несколько реализаций интерфейса:
- ListGraph, хранящий граф в виде массива списков смежности,
- MatrixGraph, хранящий граф в виде матрицы смежности,
- SetGraph, хранящий граф в виде массива хэш-таблиц/сбалансированных деревьев
  поиска,
- ArcGraph, хранящий граф в виде одного массива пар {from, to}.

Также необходимо реализовать конструктор, принимающий const IGraph&. Такой
конструктор должен скопировать переданный граф в создаваемый объект. Для
каждого класса создавайте отдельные h и cpp файлы. Число вершин графа задается
в конструкторе каждой реализации.
*/

void BFS(const IGraph &graph, int vertex,
         std::vector<bool> &visited, std::function<void(int)> &func) {
  std::queue<int> q;
  q.push(vertex);
  visited[vertex] = true;
    
  while (!q.empty()) {
    int current_vertex = q.front();
    q.pop();
    
    func(current_vertex);
    
    for (int next_vertex: graph.GetNextVertices(current_vertex)) {
      if (!visited[next_vertex]) {
        visited[next_vertex] = true;
        q.push(next_vertex);
      }
    }
  }
}

void BFS(const IGraph &graph, std::function<void(int)> func) {
  std::vector<bool> visited(graph.VerticesCount(), false);
    
  for (int i = 0; i < graph.VerticesCount(); i++) {
    if (!visited[i]) {
      BFS(graph, i, visited, func);
    }
  }
}

void DFS(const IGraph &graph, int vertex,
         std::vector<bool> &visited, std::function<void(int)> &func) {
  visited[vertex] = true;
  func(vertex);
  
  for (int next_vertex: graph.GetNextVertices(vertex)) {
    if (!visited[next_vertex]) {
      DFS(graph, next_vertex, visited, func);
    }
  }
}

void DFS(const IGraph &graph, std::function<void(int)> func) {
  std::vector<bool> visited(graph.VerticesCount(), false);
  
  for (int i = 0; i < graph.VerticesCount(); i++) {
    if (!visited[i]) {
      DFS(graph, i, visited, func);
    }
  }
}

int main() {
  ListGraph list_graph(9);

  list_graph.AddEdge(0, 1);
  list_graph.AddEdge(0, 5);
  list_graph.AddEdge(1, 2);
  list_graph.AddEdge(1, 3);
  list_graph.AddEdge(1, 5);
  list_graph.AddEdge(1, 6);
  list_graph.AddEdge(3, 2);
  list_graph.AddEdge(3, 4);
  list_graph.AddEdge(3, 6);
  list_graph.AddEdge(5, 4);
  list_graph.AddEdge(5, 6);
  list_graph.AddEdge(6, 4);
  list_graph.AddEdge(7, 8);

  auto print_vertex = [](int vertex) {
    std::cout << vertex << ' ';
  };

  std::cout << "ListGraph" << std::endl;
  std::cout << "\tVerticesCount: " << list_graph.VerticesCount() << std::endl;
  std::cout << "\tBFS: ";
  BFS(list_graph, print_vertex);
  std::cout << std::endl;
  std::cout << "\tDFS: ";
  DFS(list_graph, print_vertex);
  std::cout << std::endl;

  ArcGraph arc_graph(list_graph);
  std::cout << "ArcGraph" << std::endl;
  std::cout << "\tVerticesCount: " << arc_graph.VerticesCount() << std::endl;
  std::cout << "\tBFS: ";
  BFS(arc_graph, print_vertex);
  std::cout << std::endl;
  std::cout << "\tDFS: ";
  DFS(arc_graph, print_vertex);
  std::cout << std::endl;
  
  MatrixGraph matrix_graph(arc_graph);
  std::cout << "MatrixGraph" << std::endl;
  std::cout << "\tVerticesCount: " << matrix_graph.VerticesCount() << std::endl;
  std::cout << "\tBFS: ";
  BFS(matrix_graph, print_vertex);
  std::cout << std::endl;
  std::cout << "\tDFS: ";
  DFS(matrix_graph, print_vertex);
  std::cout << std::endl;
  
  SetGraph set_graph(matrix_graph);
  std::cout << "SetGraph" << std::endl;
  std::cout << "\tVerticesCount: " << set_graph.VerticesCount() << std::endl;
  std::cout << "\tBFS: ";
  BFS(set_graph, print_vertex);
  std::cout << std::endl;
  std::cout << "\tDFS: ";
  DFS(set_graph, print_vertex);
  std::cout << std::endl;
  
  return 0;
}
