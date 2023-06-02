#include "SetGraph.hpp"

#include <cassert>
#include <unordered_set>
#include <vector>

#include "IGraph.hpp"

SetGraph::SetGraph(std::size_t size)
    : hash_tables_(size) {
}

SetGraph::SetGraph(const IGraph& other)
    : SetGraph(other.VerticesCount()) {
  for (int from = 0; from < other.VerticesCount(); from++) {
    for (int to : other.GetNextVertices(from)) {
      hash_tables_[from].insert(to);
    }
  }
}

void SetGraph::AddEdge(int from, int to) {
  assert(0 <= from && from <= VerticesCount());
  assert(0 <= to && to <= VerticesCount());

  hash_tables_[from].insert(to);
}

int SetGraph::VerticesCount() const {
  return static_cast<int>(hash_tables_.size());
}

std::vector<int> SetGraph::GetNextVertices(int vertex) const {
  assert(0 <= vertex && vertex <= VerticesCount());

  std::vector<int> next_vertices;
  for (int to : hash_tables_[vertex]) {
    next_vertices.push_back(to);
  }

  return next_vertices;
}

std::vector<int> SetGraph::GetPrevVertices(int vertex) const {
  assert(0 <= vertex && vertex <= VerticesCount());

  std::vector<int> prev_vertices;
  for (int from = 0; from < VerticesCount(); from++) {
    for (int to : hash_tables_[from]) {
      if (to == vertex) {
        prev_vertices.push_back(from);
      }
    }
  }

  return prev_vertices;
}

