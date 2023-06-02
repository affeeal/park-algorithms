#pragma once

#include <utility>
#include <vector>

#include "IGraph.hpp"

struct ArcGraph : public IGraph {
 public:
  ArcGraph(std::size_t size);
  ArcGraph(const IGraph& other);

  void AddEdge(int from, int to) override;
  
  int VerticesCount() const override;

  std::vector<int> GetNextVertices(int vertex) const override;
  std::vector<int> GetPrevVertices(int vertex) const override;

 private:
  std::vector<std::pair<int, int>> edges_;
  std::size_t vertices_count_;

};

