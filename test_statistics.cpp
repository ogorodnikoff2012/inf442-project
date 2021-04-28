#include "statistics.h"
#include "util.h"
#include <iostream>

int main() {
  size_t N     = 1000;
  double p     = 0.005;
  int samples  = 10;
  Stats result = estimateStatsER(N, p, samples);
  std::cout << "num_of_vertex = " << N << "; p = " << p
            << "; num_of_samples = " << samples << std::endl;
  std::cout << "num_of_isolated_vertices = " << result.avg_iso
            << "; num_of_SCC = " << result.avg_scc
            << "; min_max_SCC = " << result.max_scc;
  return 0;
}