#include <cassert>
#include <climits>
#include <iostream>
#include <queue>
#include <vector>

/*
Дан невзвешенный неориентированный граф. В графе может быть несколько
кратчайших путей между какими-то вершинами. Найдите количество различных
кратчайших путей между заданными вершинами. Требуемая сложность O(V+E).

Формат ввода:

v: кол-во вершин (макс. 50000),
n: кол-во ребер(макс. 200000),
n пар реберных вершин,
пара вершин (u, w) для запроса.

Формат вывода

Количество кратчайших путей от v к w.
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

void Bfs(
    const IGraph& graph,
    int vertex,
    std::vector<int>& lengths,
    std::vector<int>& parents,
    std::vector<int>& count) {
  lengths[vertex] = 0;
  count[vertex] = 1;
  
  std::queue<int> q;
  q.push(vertex);
  while (!q.empty()) {
    int from = q.front();
    q.pop();
    
    for (int to : graph.GetNextVertices(from)) {
      if (lengths[to] > lengths[from] + 1) {
        lengths[to] = lengths[from] + 1;
        parents[to] = from;
        count[to] = count[from];
        q.push(to);
      } else if (lengths[to] == lengths[from] + 1) {
        count[to] += count[from];
      }
    }
  }
}

int main() {
  int n = 0;
  std::cin >> n;

  ListGraph list_graph(n);

  int m = 0;
  std::cin >> m;
  
  for (int i = 0; i < m; i++) {
    int from = 0, to = 0;
    std::cin >> from >> to;
    list_graph.AddEdge(from, to);
    list_graph.AddEdge(to, from);
  }

  int from = 0, to = 0;
  std::cin >> from >> to;

  std::vector<int> lengths(n, INT_MAX);
  std::vector<int> parents(n, -1);
  std::vector<int> count(n, 0);
  Bfs(list_graph,
      from,
      lengths,
      parents,
      count);

  std::cout << count[to] << std::endl;

  return 0;
}
