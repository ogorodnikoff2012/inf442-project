#ifndef INF442_STATISTICS_H
#define INF442_STATISTICS_H

#pragma once
#include "graph.h"
#include "tarjan.h"

#include <random>

class ExponentialMovingAverage {
 public:
  explicit ExponentialMovingAverage(double factor);
  ExponentialMovingAverage(double smoothing, size_t width);
  void Add(double value);
  [[nodiscard]] double Mean() const;
  [[nodiscard]] double GetFactor() const;
  void Reset();
  [[nodiscard]] bool IsEmpty() const;

 private:
  double factor_;
  double ema_{0};
  bool empty_{true};
};

class CompactStatistic {
 public:
  CompactStatistic() = default;
  explicit CompactStatistic(const std::vector<double>& values);
  virtual ~CompactStatistic() = default;

  virtual void Add(double value);

  [[nodiscard]] double Mean() const;
  [[nodiscard]] double Variance() const;
  [[nodiscard]] double Min() const;
  [[nodiscard]] double Max() const;

 private:
  size_t count_{0};
  double sum_{0};
  double sum_sq_{0};
  double min_{std::numeric_limits<double>::infinity()};
  double max_{-std::numeric_limits<double>::infinity()};
};

class Statistic : public CompactStatistic {
 public:
  Statistic() = default;
  explicit Statistic(std::vector<double> values);

  void Reserve(size_t n);
  void Sort();
  [[nodiscard]] bool IsSorted() const;

  void Add(double value) override;

  double Quantile(double level);

 private:
  std::vector<double> values_;
  bool is_sorted_{true};
};

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
  Statistic isolated;
  // double avg_iso;
  Statistic scc_count;
  Statistic scc_largest;
  // double avg_scc;
  // unsigned int max_scc;
} typedef Stats;

Stats EstimateStatsER(size_t n, double p, size_t samples);

#endif // INF442_STATISTICS_H
