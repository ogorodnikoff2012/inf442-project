#ifndef INF442_TARJAN_H
#define INF442_TARJAN_H

#include "graph.h"

std::vector<std::vector<Graph::Vertex>>
FindConnectedComponentsTarjan(const Graph& gr);

#endif // INF442_TARJAN_H
