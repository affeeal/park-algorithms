#include <cassert>
#include <climits>
#include <iostream>
#include <set>
#include <utility>
#include <vector>

/*
Требуется отыскать самый короткий маршрут между городами. Из города может
выходить дорога, которая возвращается в этот же город. Требуемое время работы
O((N + M)log N), где N – количество городов, M – известных дорог между ними.
N ≤ 10000, M ≤ 250000. Длина каждой дороги ≤ 10000.

Формат ввода
- Первая строка содержит число N – количество городов.
- Вторая строка содержит число M - количество дорог.
- Каждая следующая строка содержит описание дороги (откуда, куда, время в пути).
Все указанные дороги двусторонние. Между любыми двумя городами может быть
больше одной дороги.
- Последняя строка содержит маршрут (откуда и куда нужно доехать).

Формат вывода
Вывести длину самого короткого маршрута.
*/

using IntPair = std::pair<int, int>;

class IWeightedGraph {
 public:
  virtual ~IWeightedGraph() = default;
	
  virtual void AddEdge(int from, int to, int weight) = 0;
  
  virtual std::size_t VerticesCount() const = 0;

  virtual std::vector<IntPair> GetNextVertices(int vertex) const = 0;
  virtual std::vector<IntPair> GetPrevVertices(int vertex) const = 0;
};

class ListWeightedGraph : public IWeightedGraph {
 public:
  ListWeightedGraph(std::size_t size)
      : adjacency_lists_(size) {
  }
  
  ListWeightedGraph(const IWeightedGraph& other)
      : adjacency_lists_(other.VerticesCount()) {
    for (int i = 0; i < other.VerticesCount(); i++) {
      adjacency_lists_[i] = other.GetNextVertices(i);
    }
  }
   
  void AddEdge(int from, int to, int weight) override {
    assert(0 <= from && from <= VerticesCount());
    assert(0 <= to && to <= VerticesCount());
    
    adjacency_lists_[from].push_back(std::make_pair(to, weight));
  }
  
  std::size_t VerticesCount() const override {
    return adjacency_lists_.size();
  }

  std::vector<IntPair> GetNextVertices(int vertex) const override {
    assert(0 <= vertex && vertex <= VerticesCount());

    return adjacency_lists_[vertex];
  }
  
  std::vector<IntPair> GetPrevVertices(int vertex) const override {
    assert(0 <= vertex && vertex <= adjacency_lists_.size());
    
    std::vector<IntPair> prev_vertices;
    for (int from = 0; from < VerticesCount(); from++) {
      for (auto p : adjacency_lists_[from]) {
        if (p.first == vertex) {
          prev_vertices.push_back(std::make_pair(from, p.second));
        }
      }
    }

    return prev_vertices;
  }

 private:
  std::vector<std::vector<IntPair>> adjacency_lists_;
  
};

void Dijkstra(
    const IWeightedGraph& graph,
    int vertex,
    std::vector<int>& distances) {
  distances[vertex] = 0;
  std::set<IntPair> pq;
  pq.insert(std::make_pair(0, vertex));
  while (!pq.empty()) {
    auto [distance, from] = pq.extract(pq.begin()).value();
    for (auto [to, weight] : graph.GetNextVertices(from)) {
      if (distances[to] == INT_MAX) {
        distances[to] = distances[from] + weight;
        pq.insert(std::make_pair(distances[to], to));
      } else if (distances[to] > distances[from] + weight) {
        pq.extract(std::make_pair(distances[to], to));
        distances[to] = distances[from] + weight;
        pq.insert(std::make_pair(distances[to], to));
      }
    }
  }
}

int main() {
  std::size_t n = 0;
  std::cin >> n;

  ListWeightedGraph graph(n);

  std::size_t m = 0;
  std::cin >> m;

  for (std::size_t i = 0; i < m; i++) {
    int from = 0, to = 0, weight = 0;
    std::cin >> from >> to >> weight;
    graph.AddEdge(from, to, weight);
    if (from != to) {
      graph.AddEdge(to, from, weight);
    }
  }

  int from = 0, to = 0;
  std::cin >> from >> to;

  std::vector<int> distances(n, INT_MAX);
  Dijkstra(graph, from, distances);
  std::cout << distances[to] << std::endl;

  return 0;
}
