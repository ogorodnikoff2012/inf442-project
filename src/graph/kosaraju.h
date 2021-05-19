#ifndef INF442_KOSARAJU_H
#define INF442_KOSARAJU_H

#include "graph.h"

namespace graph {

std::vector<std::vector<Graph::Vertex>>
FindConnectedComponentsKosaraju(const Graph& gr, bool show_progress = false);

} // namespace graph

#endif // INF442_KOSARAJU_H
