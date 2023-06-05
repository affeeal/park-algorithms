#include <cassert>
#include <functional>
#include <iostream>
#include <vector>

/*
Вариант 2. Проверить, является ли неориентированный граф деревом.

Формат ввода
Первая строка содержит число N – количество вершин.
Вторая строка содержит число M - количество ребер.
Каждая следующая строка содержит ребро (откуда, куда).

Формат вывода
1 - если граф дерево
0 - если не дерево
*/

class IGraph {
 public:
  virtual ~IGraph() = default;
	
  virtual void AddEdge(int from, int to) = 0;
  
  virtual std::size_t VerticesCount() const = 0;

  virtual std::vector<int> GetNextVertices(int vertex) const = 0;
  virtual std::vector<int> GetPrevVertices(int vertex) const = 0;
};

class ListGraph : public IGraph {
 public:
  ListGraph(std::size_t size);
  ListGraph(const IGraph& other);
   
  void AddEdge(int from, int to) override;
  
  std::size_t VerticesCount() const override;

  std::vector<int> GetNextVertices(int vertex) const override;
  std::vector<int> GetPrevVertices(int vertex) const override;

 private:
  std::vector<std::vector<int>> adjacency_lists_;
  
};

ListGraph::ListGraph(std::size_t size)
    : adjacency_lists_(size) {
}

ListGraph::ListGraph(const IGraph& other)
    : adjacency_lists_(other.VerticesCount()) {
  for (int i = 0; i < other.VerticesCount(); i++) {
    adjacency_lists_[i] = other.GetNextVertices(i);
  }
}

void ListGraph::AddEdge(int from, int to) {
  assert(0 <= from && from <= VerticesCount());
  assert(0 <= to && to <= VerticesCount());
  
  adjacency_lists_[from].push_back(to);
}

std::size_t ListGraph::VerticesCount() const {
  return adjacency_lists_.size();
}

std::vector<int> ListGraph::GetNextVertices(int vertex) const {
  assert(0 <= vertex && vertex <= VerticesCount());

  return adjacency_lists_[vertex];
}

std::vector<int> ListGraph::GetPrevVertices(int vertex) const {
  assert(0 <= vertex && vertex <= adjacency_lists_.size());
  
  std::vector<int> prev_vertices;
  for (int from = 0; from < VerticesCount(); from++) {
    for (int to : adjacency_lists_[from]) {
      if (to == vertex) {
        prev_vertices.push_back(from);
      }
    }
  }

  return prev_vertices;
}

bool DFS(const IGraph &graph, int from, int parent,
    std::vector<bool>& visited) {
  visited[from] = true;
  
  for (int to : graph.GetNextVertices(from)) {
    if (!visited[to]) {
      if (DFS(graph, to, from, visited)) {
        return true;
      }
    } else if (to != parent) {
      return true;
    }
  }

  return false;
}

int main() {
  std::size_t n = 0;
  std::cin >> n;

  ListGraph graph(n);

  std::size_t m = 0;
  std::cin >> m;

  for (std::size_t i = 0; i < m; i++) {
    int from = 0, to = 0;
    std::cin >> from >> to;
    graph.AddEdge(from, to);
    graph.AddEdge(to, from);
  }
  
  assert(n >= 1);
  std::vector<bool> visited(n);
  bool has_back_edges = DFS(graph, 0, -1, visited);
  if (has_back_edges) {
    std::cout << '0' << std::endl;
    return 0;
  }

  for (int from = 0; from < n; from++) {
    // если обошли не все вершины
    if (!visited[from]) {
      std::cout << '0' << std::endl;
      return 0;
    }
  }
  
  std::cout << '1' << std::endl;
  return 0;
}
