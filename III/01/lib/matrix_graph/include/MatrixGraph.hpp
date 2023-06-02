#pragma once

#include "IGraph.hpp"

struct MatrixGraph : public IGraph {
 public:
  MatrixGraph(std::size_t size);
  MatrixGraph(const IGraph& other);
  
  void AddEdge(int from, int to) override;
  
  int VerticesCount() const override;

  std::vector<int> GetNextVertices(int vertex) const override;
  std::vector<int> GetPrevVertices(int vertex) const override;

 private:
  std::vector<std::vector<bool>> adjacency_matrix_;
  
};

