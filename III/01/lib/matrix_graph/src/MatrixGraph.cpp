#include "MatrixGraph.hpp"

#include <cassert>
#include <vector>

#include "IGraph.hpp"

MatrixGraph::MatrixGraph(std::size_t size)
    : adjacency_matrix_(size) {
  for (std::vector<bool>& row : adjacency_matrix_) {
    row.resize(size);
  }
}

MatrixGraph::MatrixGraph(const IGraph& other)
    : MatrixGraph(other.VerticesCount()) {
  for (int from = 0; from < other.VerticesCount(); from++) {
    for (int to : other.GetNextVertices(from)) {
      adjacency_matrix_[from][to] = true;
    }
  }
}

void MatrixGraph::AddEdge(int from, int to) {
  assert(0 <= from && from <= VerticesCount());
  assert(0 <= to && to <= VerticesCount());

  adjacency_matrix_[from][to] = true;
}

int MatrixGraph::VerticesCount() const {
  return static_cast<int>(adjacency_matrix_.size());
}

std::vector<int> MatrixGraph::GetNextVertices(int vertex) const {
  assert(0 <= vertex && vertex <= VerticesCount());

  std::vector<int> next_vertices;
  for (int to = 0; to < VerticesCount(); to++) {
    if (adjacency_matrix_[vertex][to]) {
      next_vertices.push_back(to);
    }
  }

  return next_vertices;
}

std::vector<int> MatrixGraph::GetPrevVertices(int vertex) const {
  assert(0 <= vertex && vertex <= VerticesCount());

  std::vector<int> prev_vertices;
  for (int from = 0; from < VerticesCount(); from++) {
    for (int to = 0; to < VerticesCount(); to++) {
      if (adjacency_matrix_[from][to]) {
        prev_vertices.push_back(from);
      }
    }
  }

  return prev_vertices;
}

