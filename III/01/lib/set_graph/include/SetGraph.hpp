#pragma once

#include <unordered_set>

#include "IGraph.hpp"

class SetGraph : public IGraph {
 public:
  SetGraph(std::size_t size);
  SetGraph(const IGraph& other);

  void AddEdge(int from, int to) override;
  
  int VerticesCount() const override;

  std::vector<int> GetNextVertices(int vertex) const override;
  std::vector<int> GetPrevVertices(int vertex) const override;

 private:
  std::vector<std::unordered_set<int>> hash_tables_;

};

