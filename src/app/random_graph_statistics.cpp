#include "../graph/random_graph.h"
#include "../stats/statistics.h"

#include <iostream>

void PrintStatistics(const char* title, Statistic& statistics) {
  std::cout << title << ":\n";
  std::cout << "  Average: " << statistics.Mean() << "+-"
            << std::sqrt(statistics.Variance()) << '\n'
            << "  Min:     " << statistics.Min() << '\n'
            << "  Q25:     " << statistics.Quantile(0.25) << '\n'
            << "  Q50:     " << statistics.Quantile(0.5) << '\n'
            << "  Q75:     " << statistics.Quantile(0.75) << '\n'
            << "  Max:     " << statistics.Max() << '\n';
}

int main() {
  const size_t N       = 1000;
  const double p       = 0.002;
  const size_t samples = 100;
  std::cout << "num_of_vertex = " << N << "; p = " << p
            << "; num_of_samples = " << samples << std::endl;

  Stats result = EstimateStatsER(N, p, samples);
  PrintStatistics("Number of isolated vertices", result.isolated);
  PrintStatistics("Number of SCC", result.scc_count);
  PrintStatistics("Largest SCC", result.scc_largest);
  return 0;
}