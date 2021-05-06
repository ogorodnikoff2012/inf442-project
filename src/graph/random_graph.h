#ifndef INF442_RANDOM_GRAPH_H
#define INF442_RANDOM_GRAPH_H

#include "../stats/statistics.h"
#include "graph.h"

namespace graph {

template <class Generator>
Graph GenerateER(size_t n, double p, Generator& g) {
  Graph res(n);

  std::uniform_real_distribution<double> distr(0.0, 1.0);

  for (Graph::Vertex vertex1 = 0; vertex1 < n; vertex1++) {
    for (Graph::Vertex vertex2 = 0; vertex2 < n; vertex2++) {
      if (vertex1 != vertex2 && distr(g) <= p) {
        res.AddEdge(vertex1, vertex2);
      }
    }
  }

  return res;
}

struct Stats {
  stats::Statistic isolated;
  // double avg_iso;
  stats::Statistic scc_count;
  stats::Statistic scc_largest;
  // double avg_scc;
  // unsigned int max_scc;
} typedef Stats;

Stats EstimateStatsER(size_t n, double p, size_t samples);

} // namespace graph

#endif // INF442_RANDOM_GRAPH_H
