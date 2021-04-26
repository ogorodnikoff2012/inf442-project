#include "tarjan.h"
#include "util.h"

#include <fstream>

std::vector<std::vector<Graph::Vertex>>
FindConnectedComponentsTarjan(const Graph& gr) {
  class TarjanVisitor {
   public:
    explicit TarjanVisitor(std::vector<Graph::Vertex>* component)
        : component_(component) {}
    TarjanVisitor(const TarjanVisitor&) = default;

    void OnVertexEnter(Graph::Vertex vertex) { component_->push_back(vertex); }

    void OnVertexExit(Graph::Vertex vertex) {}
    void OnEdgeDiscover(Graph::Vertex from, Graph::Vertex to) {}

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

int main(int argc, char* argv[]) {
  std::ifstream fin("tarjan.in");
  std::ofstream fout("tarjan.out");

  Graph gr = ReadFromEdgeList(fin);
  fin.close();

  auto components = FindConnectedComponentsTarjan(gr);
  fout << "Components:\n";
  for (const auto& component : components) {
    fout << component << '\n';
  }
  fout.close();

  return 0;
}
