#include "kosaraju.h"
#include "../util/progress_bar.h"
#include "../util/util.h"

#include <fstream>
#include <memory>

namespace graph {

std::vector<std::vector<Graph::Vertex>>
FindConnectedComponentsKosaraju(const Graph& gr, bool show_progress) {
  class KosarajuVisitor {
   public:
    explicit KosarajuVisitor(std::vector<Graph::Vertex>* component,
                             std::shared_ptr<util::ProgressBar> bar)
        : component_(component)
        , bar_(std::move(bar)) {}

    void OnVertexEnter(Graph::Vertex vertex) {
      component_->push_back(vertex);
      if (bar_) {
        bar_->IncrementCounter();
      }
    }

    void OnVertexExit(Graph::Vertex) {}
    void OnEdgeDiscover(Graph::Vertex, Graph::Vertex) {}

   private:
    std::vector<Graph::Vertex>* component_;
    std::shared_ptr<util::ProgressBar> bar_;
  };

  auto vertices = TopologicalSort(gr, show_progress);
  if (show_progress) {
    std::cerr << "Transposing\n";
  }
  auto transposed = gr.Transposed();

  std::shared_ptr<util::ProgressBar> bar{nullptr};
  if (show_progress) {
    bar.reset(new util::ProgressBar(std::cerr));
    bar->SetLimit(gr.VertexCount());
    bar->Out() << "Find components";
  }

  std::unordered_set<Graph::Vertex> visited;
  std::vector<std::vector<Graph::Vertex>> components;

  for (auto it = vertices.rbegin(); it != vertices.rend(); ++it) {
    auto vertex = *it;
    if (visited.find(vertex) == visited.end()) {
      components.emplace_back();
      DFSHelper(transposed, visited, vertex,
                KosarajuVisitor(&components.back(), bar));
    }
  }

  return components;
}

} // namespace graph