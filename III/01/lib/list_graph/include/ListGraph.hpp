#pragma once

#include "IGraph.hpp"

struct ListGraph : public IGraph {
 public:
  ListGraph(std::size_t size);
  ListGraph(const IGraph& other);
   
  void AddEdge(int from, int to) override;
  
  int VerticesCount() const override;

  std::vector<int> GetNextVertices(int vertex) const override;
  std::vector<int> GetPrevVertices(int vertex) const override;

 private:
  std::vector<std::vector<int>> adjacency_lists_;
  
};

