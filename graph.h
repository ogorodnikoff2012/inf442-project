#ifndef INF442_GRAPH_H
#define INF442_GRAPH_H

#include <istream>
#include <unordered_set>
#include <vector>

class Graph {
 public:
  using Vertex = size_t;

  explicit Graph(size_t vertices);

  void AddEdge(Vertex from, Vertex to);

  size_t VertexCount() const;

  const std::vector<Vertex>& GetEdges(Vertex vertex) const;

  Graph Transposed() const;

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

std::vector<Graph::Vertex> TopologicalSort(const Graph& gr);

#endif // INF442_GRAPH_H
