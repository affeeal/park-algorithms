#include "ArcGraph.hpp"

#include <cassert>
#include <utility>
#include <vector>

#include "IGraph.hpp"

ArcGraph::ArcGraph(std::size_t size)
    : vertices_count_(size) {
}

ArcGraph::ArcGraph(const IGraph& other)
    : vertices_count_(other.VerticesCount()) {
  for (int from = 0; from < vertices_count_; from++) {
    for (int to : other.GetNextVertices(from)) {
      edges_.push_back(std::make_pair(from, to));
    }
  }
}
    
void ArcGraph::AddEdge(int from, int to) {
  assert(0 <= from && from <= VerticesCount());
  assert(0 <= to && to <= VerticesCount());

  edges_.push_back(std::make_pair(from, to));
}

int ArcGraph::VerticesCount() const {
  return static_cast<int>(vertices_count_);
}

std::vector<int> ArcGraph::GetNextVertices(int vertex) const {
  assert(0 <= vertex && vertex <= VerticesCount());

  std::vector<int> next_vertices;
  for (const auto& edge : edges_) {
    if (edge.first == vertex) {
      next_vertices.push_back(edge.second);
    }
  }

  return next_vertices;
}

std::vector<int> ArcGraph::GetPrevVertices(int vertex) const {
  assert(0 <= vertex && vertex <= VerticesCount());

  std::vector<int> prev_vertices;
  for (const auto& edge : edges_) {
    if (edge.second == vertex) {
      prev_vertices.push_back(edge.first);
    }
  }

  return prev_vertices;
}

