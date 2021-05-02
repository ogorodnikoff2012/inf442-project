#ifndef INF442_STATISTICS_H
#define INF442_STATISTICS_H

#pragma once

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
  void SetFactor(double factor);
  void SetFactor(double smoothing, size_t width);

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

#endif // INF442_STATISTICS_H
