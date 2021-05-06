#ifndef INF442_TARJAN_H
#define INF442_TARJAN_H

#include "graph.h"

namespace graph {

std::vector<std::vector<Graph::Vertex>>
FindConnectedComponentsTarjan(const Graph& gr, bool show_progress = false);

} // namespace graph

#endif // INF442_TARJAN_H
