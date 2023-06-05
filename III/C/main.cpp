#include <cassert>
#include <climits>
#include <iostream>
#include <set>
#include <utility>
#include <vector>

/*
Вариант 2. Рику необходимо попасть на межвселенную конференцию. За каждую
телепортацию он платит бутылками лимонада, поэтому хочет потратить их на
дорогу как можно меньше (он же всё-таки на конференцию едет!). Однако после K
перелетов подряд Рика начинает сильно тошнить, и он ложится спать на день. Ему
известны все существующие телепортации. Теперь Рик хочет найти путь (наименьший
по стоимости в бутылках лимонада), учитывая, что телепортация не занимает
времени, а до конференции осталось 10 минут (то есть он может совершить не
более K перелетов)!

Формат ввода

В первой строке находятся количество вселенных N, количество существующих
телепортаций M, количество перелетов подряд K, номер вселенной Рика S, номер
вселенной конференции F (2 ≤ N ≤ 300, 1 ≤ M ≤ 10^5, 1 ≤ K ≤ 300, 1 ≤ S ≤ N,
1 ≤ F ≤ N).

Далее идет M строк, задающих телепортации. i-я строка содержит три натуральных
числа: S_i, F_i и P_i, где S_i - номер вселенной, из которой ведет i-я
телепортация, Fi - номер вселенной, в которую ведет i-я телепортация, Pi -
стоимость i-й телепортации. 1 ≤ S_i ≤ N, 1 ≤ F_i ≤ N, 1 ≤ P_i ≤ 10^6.

Формат вывода

Выведите одно число - минимальную стоимость пути, подходящего для профессора.
Если профессор не сможет за K ночей добраться до конференции, выведите -1.
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
    std::vector<int>& distances,
    std::vector<int>& parents) {
  distances[vertex] = 0;
  std::set<IntPair> s;
  s.insert(std::make_pair(0, vertex));
  while (!s.empty()) {
    auto [distance, from] = s.extract(s.begin()).value();
    for (auto [to, weight] : graph.GetNextVertices(from)) {
      if (distances[to] == INT_MAX) {
        distances[to] = distances[from] + weight;
        parents[to] = from;
        s.insert(std::make_pair(distances[to], to));
      } else if (distances[to] > distances[from] + weight) {
        s.extract(std::make_pair(distances[to], to));
        distances[to] = distances[from] + weight;
        parents[to] = from;
        s.insert(std::make_pair(distances[to], to));
      }
    }
  }
}

int main() {
  std::size_t n = 0, m = 0, k = 0; // вершины, рёбра, перемещения
  std::cin >> n >> m >> k;

  ListWeightedGraph graph(n);

  int s = 0, f = 0; // старт, финиш
  std::cin >> s >> f;
  
  for (std::size_t i = 0; i < m; i++) {
    int from = 0, to = 0, weight = 0;
    std::cin >> from >> to >> weight;
    graph.AddEdge(from - 1, to - 1, weight);
  }

  std::vector<int> distances(n, INT_MAX);
  std::vector<int> parents(n, -1);
  Dijkstra(graph, s - 1, distances, parents);

  int transitions_left = k;
  int parent = f - 1;
  while (transitions_left) {
    parent = parents[parent];
    if (parent == s - 1) {
      break;
    }
    
    transitions_left--;
  }

  if (parent == s - 1) {
    std::cout << distances[f - 1] << std::endl;
  } else {
    std::cout << -1 << std::endl;
  }

  return 0;
}
