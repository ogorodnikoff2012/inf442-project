#include "../graph/random_graph.h"
#include "../stats/statistics.h"

#include <cstring>
#include <iostream>

void PrintStatistics(const char* title, stats::Statistic& statistics) {
  std::cout << title << ":\n";
  std::cout << "  Average: " << statistics.Mean() << "+-"
            << std::sqrt(statistics.Variance()) << '\n'
            << "  Min:     " << statistics.Min() << '\n'
            << "  Q25:     " << statistics.Quantile(0.25) << '\n'
            << "  Q50:     " << statistics.Quantile(0.5) << '\n'
            << "  Q75:     " << statistics.Quantile(0.75) << '\n'
            << "  Max:     " << statistics.Max() << '\n';
}

void PrintQuietStatistics(const char* title, stats::Statistic& statistics) {
  std::cout << title << " = {'avg': " << statistics.Mean()
            << ", 'err': " << std::sqrt(statistics.Variance())
            << ", 'min': " << statistics.Min()
            << ", 'q25': " << statistics.Quantile(0.25)
            << ", 'q50': " << statistics.Quantile(0.5)
            << ", 'q75': " << statistics.Quantile(0.75)
            << ", 'max': " << statistics.Max() << "}\n";
}

struct Args {
  size_t N;
  double p;
  size_t samples;
  bool quiet;
};

bool ParseArgs(Args* args, int argc, char* argv[]) {
  if (argc < 4 || argc > 5) {
    return false;
  }

  args->quiet = false;
  if (argc == 5) {
    if (std::strcmp(argv[4], "quiet") != 0) {
      return false;
    }
    args->quiet = true;
  }

  char* ptr = nullptr;
  {
    int64_t val = std::strtol(argv[1], &ptr, 10);
    if (ptr == argv[1]) {
      return false;
    }
    if (val <= 0) {
      return false;
    }
    args->N = val;
  }
  {
    ptr        = nullptr;
    double val = std::strtod(argv[2], &ptr);
    if (ptr == argv[2]) {
      return false;
    }
    if (val < 0 || val > 1) {
      return false;
    }
    args->p = val;
  }
  {
    ptr         = nullptr;
    int64_t val = std::strtol(argv[3], &ptr, 10);
    if (ptr == argv[3]) {
      return false;
    }
    if (val <= 0) {
      return false;
    }
    args->samples = val;
  }
  return true;
}

int main(int argc, char* argv[]) {
  Args args{0, 0, 0, false};
  if (!ParseArgs(&args, argc, argv)) {
    std::cerr << "Usage: " << argv[0] << " N p num_samples [quiet]\n";
    return 1;
  }

  if (!args.quiet) {
    std::cout << "num_of_vertex = " << args.N << "; p = " << args.p
              << "; num_of_samples = " << args.samples << std::endl;
  }

  graph::Stats result = graph::EstimateStatsER(args.N, args.p, args.samples);
  if (args.quiet) {
    PrintQuietStatistics("isolated", result.isolated);
    PrintQuietStatistics("scc_count", result.scc_count);
    PrintQuietStatistics("scc_largest", result.scc_largest);
    PrintQuietStatistics("scc_2largest", result.scc_2largest);
  } else {
    PrintStatistics("Number of isolated vertices", result.isolated);
    PrintStatistics("Number of SCC", result.scc_count);
    PrintStatistics("Largest SCC", result.scc_largest);
    PrintStatistics("Second largest SCC", result.scc_2largest);
  }
  return 0;
}