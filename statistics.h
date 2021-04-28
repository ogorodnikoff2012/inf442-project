#ifndef INF442_STATISTICS_H
#define INF442_STATISTICS_H

#pragma once
#include "graph.h"
#include "tarjan.h"
#include <random>

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
  double avg_iso;
  double avg_scc;
  unsigned int max_scc;
} typedef Stats;

Stats estimateStatsER(size_t n, double p, int samples);

#endif // INF442_STATISTICS_H
