#ifndef INF442_GRAPH_H
#define INF442_GRAPH_H

#include "progress_bar.h"

#include <istream>
#include <stack>
#include <unordered_set>
#include <vector>

class Graph {
 public:
  using Vertex = size_t;

  explicit Graph(size_t vertices);

  void AddEdge(Vertex from, Vertex to);

  [[nodiscard]] size_t VertexCount() const;

  [[nodiscard]] const std::vector<Vertex>& GetEdges(Vertex vertex) const;

  [[nodiscard]] Graph Transposed() const;

 private:
  std::vector<std::vector<Vertex>> edges_;
};

/* File format:
 * <vertex_count> <edge_count>
 * <from_0> <to_0>
 * ...
 * <from_(edge_count - 1)> <to_(edge_count - 1)>
 *
 * All <from_*> and <to_*> must be non-negative integers less than
 * <vertex_count>
 * */
Graph ReadFromEdgeList(std::istream& in);

/*
template <class Visitor>
void DFSHelper(const Graph& gr,
               std::unordered_set<Graph::Vertex>& visited_vertices,
               Graph::Vertex vertex, Visitor visitor) {
  visitor.OnVertexEnter(vertex);

  visited_vertices.insert(vertex);
  for (auto next : gr.GetEdges(vertex)) {
    visitor.OnEdgeDiscover(vertex, next);
    if (visited_vertices.find(next) == visited_vertices.end()) {
      DFSHelper(gr, visited_vertices, next, visitor);
    }
  }

  visitor.OnVertexExit(vertex);
}
 */

template <class Visitor>
void DFSHelper(const Graph& gr,
               std::unordered_set<Graph::Vertex>& visited_vertices,
               Graph::Vertex vertex, Visitor visitor) {
  struct Frame {
    Graph::Vertex v;
    size_t pos;
  };
  std::vector<Frame> stack;
  // stack.reserve(gr.VertexCount());

  stack.push_back(Frame{vertex, 0});
  visitor.OnVertexEnter(vertex);
  visited_vertices.insert(vertex);

  while (!stack.empty()) {
    auto& last_frame = stack.back();
    if (last_frame.pos >= gr.GetEdges(last_frame.v).size()) {
      visitor.OnVertexExit(last_frame.v);
      stack.pop_back();
      continue;
    }

    auto next = gr.GetEdges(last_frame.v)[last_frame.pos++];
    visitor.OnEdgeDiscover(last_frame.v, next);
    if (visited_vertices.find(next) == visited_vertices.end()) {
      visitor.OnVertexEnter(next);
      stack.push_back(Frame{next, 0});
      visited_vertices.insert(next);
    }
  }
}

template <class Visitor>
void DFS(const Graph& gr, Graph::Vertex vertex, Visitor visitor) {
  std::unordered_set<Graph::Vertex> visited_vertices;
  DFSHelper(gr, visited_vertices, vertex, visitor);
}

template <class Visitor>
void DFS(const Graph& gr, Visitor visitor) {
  std::unordered_set<Graph::Vertex> visited_vertices;

  for (Graph::Vertex vertex = 0; vertex < gr.VertexCount(); ++vertex) {
    if (visited_vertices.find(vertex) == visited_vertices.end()) {
      DFSHelper(gr, visited_vertices, vertex, visitor);
    }
  }
}

std::vector<Graph::Vertex> TopologicalSort(const Graph& gr,
                                           bool show_progress = false);

#endif // INF442_GRAPH_H
