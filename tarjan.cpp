#include "tarjan.h"
#include "util.h"

#include <fstream>

std::vector<std::vector<Graph::Vertex>>
FindConnectedComponentsTarjan(const Graph& gr) {
  class TarjanVisitor {
   public:
    explicit TarjanVisitor(std::vector<Graph::Vertex>* component)
        : component_(component) {}

    void OnVertexEnter(Graph::Vertex vertex) { component_->push_back(vertex); }

    void OnVertexExit(Graph::Vertex) {}
    void OnEdgeDiscover(Graph::Vertex, Graph::Vertex) {}

   private:
    std::vector<Graph::Vertex>* component_;
  };

  auto vertices   = TopologicalSort(gr);
  auto transposed = gr.Transposed();

  std::unordered_set<Graph::Vertex> visited;
  std::vector<std::vector<Graph::Vertex>> components;

  for (auto it = vertices.rbegin(); it != vertices.rend(); ++it) {
    auto vertex = *it;
    if (visited.find(vertex) == visited.end()) {
      components.emplace_back();
      DFSHelper(transposed, visited, vertex, TarjanVisitor(&components.back()));
    }
  }

  return components;
}
