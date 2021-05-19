#include "random_graph.h"
#include "../util/progress_bar.h"
#include "tarjan.h"

#include <iostream>

namespace graph {

Stats EstimateStatsER(size_t n, double p, size_t samples) {

  std::random_device rd;
  std::default_random_engine generator(rd());

  util::ProgressBar bar(std::cerr);
  bar.SetLimit(samples);

  Stats results{};
  results.isolated.Reserve(samples);
  results.scc_count.Reserve(samples);
  results.scc_largest.Reserve(samples);

  for (size_t i = 0; i < samples; i++, bar.IncrementCounter()) {
    Graph res       = GenerateER(n, p, generator);
    Graph tr_res    = res.Transposed();
    auto components = FindConnectedComponentsTarjan(res);

    results.scc_count.Add(components.size());

    unsigned int max_comp = 0;
    int index_max = 0;
    int loop = 0;
    for (const auto& comp : components) {
      auto comp_size = comp.size();
      if (comp_size > max_comp) {
        max_comp = comp_size;
        index_max = loop;
      }
      loop++;
    }
    results.scc_largest.Add(max_comp);

    unsigned int second_max = 0;
    loop = 0;
    for (const auto& comp : components) {
      auto comp_size = comp.size();
      if (comp_size > second_max && loop != index_max) {
        second_max = comp_size;
      }
      loop++;
    }
    results.scc_2largest.Add(second_max);

    size_t isolated = 0;
    for (Graph::Vertex vertex = 0; vertex < n; vertex++) {

      if (res.GetEdges(vertex).empty() && tr_res.GetEdges(vertex).empty()) {
        isolated++;
      }
    }
    results.isolated.Add(isolated);
  }

  // results.avg_scc /= (double)samples;
  // results.avg_iso /= (double)samples;

  return results;
}

} // namespace graph