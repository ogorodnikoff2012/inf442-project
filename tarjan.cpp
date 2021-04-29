#include "tarjan.h"
#include "progress_bar.h"
#include "util.h"

#include <fstream>

std::vector<std::vector<Graph::Vertex>>
FindConnectedComponentsTarjan(const Graph& gr, bool show_progress) {
  class TarjanVisitor {
   public:
    explicit TarjanVisitor(std::vector<Graph::Vertex>* component,
                           ProgressBar* bar)
        : component_(component)
        , bar_(bar) {}

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
    ProgressBar* bar_;
  };

  auto vertices = TopologicalSort(gr, show_progress);
  if (show_progress) {
    std::cerr << "Transposing\n";
  }
  auto transposed = gr.Transposed();

  ProgressBar* bar = nullptr;
  if (show_progress) {
    bar = new ProgressBar(std::cerr);
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
                TarjanVisitor(&components.back(), bar));
    }
  }

  delete bar;

  return components;
}
