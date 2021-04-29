#include "graph.h"
#include "progress_bar.h"

#include <cassert>
#include <iostream>
#include <sstream>

Graph::Graph(size_t vertices)
    : edges_(vertices) {}

void Graph::AddEdge(Graph::Vertex from, Graph::Vertex to) {
  assert(from < VertexCount() && to < VertexCount());

  edges_[from].push_back(to);
}

size_t Graph::VertexCount() const { return edges_.size(); }

const std::vector<Graph::Vertex>& Graph::GetEdges(Graph::Vertex vertex) const {
  assert(vertex < VertexCount());

  return edges_[vertex];
}

Graph Graph::Transposed() const {
  Graph result(VertexCount());

  for (Vertex from = 0; from < VertexCount(); ++from) {
    for (Vertex to : edges_[from]) {
      result.AddEdge(to, from);
    }
  }

  return result;
}

Graph ReadFromEdgeList(std::istream& in) {
  size_t vertex_count;
  if (!(in >> vertex_count)) {
    throw std::runtime_error("Failed to read <vertex_count>");
  }
  Graph result(vertex_count);

  size_t edge_count;
  if (!(in >> edge_count)) {
    throw std::runtime_error("Failed to read <edge_count>");
  }

  ProgressBar bar(std::cerr);
  bar.SetLimit(edge_count);

  for (size_t i = 0; i < edge_count; ++i, bar.IncrementCounter()) {
    size_t from, to;
    if (!(in >> from >> to)) {
      std::stringstream ss;
      ss << "Failed to read <from_" << i << "> <to_" << i << ">";
      throw std::runtime_error(ss.str());
    }

    if (from >= vertex_count || to >= vertex_count) {
      std::stringstream ss;
      ss << "Bad values of <from_" << i << "> and <to_" << i << ">";
      throw std::runtime_error(ss.str());
    }

    result.AddEdge(from, to);
  }

  return result;
}

std::vector<Graph::Vertex> TopologicalSort(const Graph& gr,
                                           bool show_progress) {
  class TopSortVisitor {
   public:
    explicit TopSortVisitor(std::vector<Graph::Vertex>* result,
                            ProgressBar* bar)
        : result_(result)
        , bar_(bar) {}

    void OnVertexEnter(Graph::Vertex) {
      if (bar_) {
        bar_->IncrementCounter();
      }
    }
    void OnVertexExit(Graph::Vertex vertex) { result_->push_back(vertex); }
    void OnEdgeDiscover(Graph::Vertex, Graph::Vertex) {}

   private:
    std::vector<Graph::Vertex>* result_;
    ProgressBar* bar_;
  };

  ProgressBar* bar = nullptr;
  if (show_progress) {
    bar = new ProgressBar(std::cerr);
    bar->SetLimit(gr.VertexCount());
    bar->Out() << "Topological sort";
  }

  std::vector<Graph::Vertex> result;
  DFS(gr, TopSortVisitor(&result, bar));
  delete bar;

  return result;
}
