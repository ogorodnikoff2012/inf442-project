#include <iostream>
#include "util.h"
#include "statistics.h"

int main() {
 size_t N = 1000;
 double p = 1;
 int samples = 100;
 Stats result = estimateStatsER(N, p, samples);
 std::cout << "num_of_vertex = " << N << "; p = " << p << "; num_of_samples = " << samples << std::endl;
 std::cout << "num_of_isolated_vertices = " << result.avg_iso
           << "; num_of_SCC = " << result.avg_scc
           << "; min_max_SCC = " << result.max_scc;
 return 0;
}