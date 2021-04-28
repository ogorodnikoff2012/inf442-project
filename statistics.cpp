#include "statistics.h"

Stats estimateStatsER(size_t n, double p, int samples) {

  std::random_device rd;
  std::default_random_engine generator(rd());

  Stats results = {0, 0, 0};

  for (int i = 0; i < samples; i++) {
    Graph res       = GenerateER(n, p, generator);
    Graph tr_res    = res.Transposed();
    auto components = FindConnectedComponentsTarjan(res);

    results.avg_scc += (double)components.size();
    unsigned int max_comp = 0;
    for (const auto& comp : components) {
      auto comp_size = comp.size();
      if (comp_size > max_comp)
        max_comp = comp_size;
    }
    if (results.max_scc == 0 || results.max_scc > max_comp)
      results.max_scc = max_comp;

    for (Graph::Vertex vertex = 0; vertex < n; vertex++) {

      if (res.GetEdges(vertex).size() == 0 &&
          tr_res.GetEdges(vertex).size() == 0)
        results.avg_iso++;
    }
  }

  results.avg_scc /= (double)samples;
  results.avg_iso /= (double)samples;

  return results;
}