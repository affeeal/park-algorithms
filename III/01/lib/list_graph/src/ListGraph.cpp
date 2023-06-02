#include "ListGraph.hpp"

#include <cassert>
#include <vector>

#include "IGraph.hpp"

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

int ListGraph::VerticesCount() const {
  return static_cast<int>(adjacency_lists_.size());
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

