#include "statistics.h"
#include "progress_bar.h"

#include <algorithm>
#include <iostream>

Stats EstimateStatsER(size_t n, double p, size_t samples) {

  std::random_device rd;
  std::default_random_engine generator(rd());

  ProgressBar bar(std::cerr);
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
    for (const auto& comp : components) {
      auto comp_size = comp.size();
      if (comp_size > max_comp) {
        max_comp = comp_size;
      }
    }
    results.scc_largest.Add(max_comp);

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

CompactStatistic::CompactStatistic(const std::vector<double>& values) {
  for (double val : values) {
    CompactStatistic::Add(val);
  }
}

double CompactStatistic::Mean() const { return sum_ / count_; }
double CompactStatistic::Min() const { return min_; }
double CompactStatistic::Max() const { return max_; }

void CompactStatistic::Add(double value) {
  ++count_;
  sum_ += value;
  sum_sq_ += value * value;
  min_ = std::min(min_, value);
  max_ = std::max(max_, value);
}
double CompactStatistic::Variance() const {
  if (count_ < 2) {
    return std::numeric_limits<double>::quiet_NaN();
  }
  return (sum_sq_ - sum_ * sum_ / count_) / (count_ - 1);
}

double Statistic::Quantile(double level) {
  if (values_.empty()) {
    return std::numeric_limits<double>::quiet_NaN();
  }

  const double k = std::floor(level * (values_.size() - 1));
  const double m = level * values_.size();

  size_t index;
  if (k + 1 - m < 0) {
    index = k + 1;
  } else {
    index = k;
  }
  if (index >= values_.size()) {
    index = values_.size() - 1;
  }

  if (!is_sorted_) {
    std::nth_element(values_.begin(), values_.begin() + index, values_.end());
  }
  return values_[index];
}

Statistic::Statistic(std::vector<double> values)
    : CompactStatistic(values)
    , values_(std::move(values)) {
  is_sorted_ = std::is_sorted(values_.begin(), values_.end());
}

void Statistic::Sort() {
  if (!is_sorted_) {
    std::sort(values_.begin(), values_.end());
    is_sorted_ = true;
  }
}

bool Statistic::IsSorted() const { return is_sorted_; }

void Statistic::Add(double value) {
  is_sorted_ &= value >= Max();
  values_.push_back(value);
  CompactStatistic::Add(value);
}
void Statistic::Reserve(size_t n) { values_.reserve(n); }

ExponentialMovingAverage::ExponentialMovingAverage(double factor)
    : factor_(factor) {}

ExponentialMovingAverage::ExponentialMovingAverage(double smoothing,
                                                   size_t width)
    : ExponentialMovingAverage(smoothing / (width + 1)) {}

void ExponentialMovingAverage::Add(double value) {
  if (empty_) {
    ema_   = value;
    empty_ = false;
  } else {
    ema_ += factor_ * (value - ema_);
  }
}

double ExponentialMovingAverage::Mean() const { return ema_; }

double ExponentialMovingAverage::GetFactor() const { return factor_; }

void ExponentialMovingAverage::Reset() {
  ema_   = 0;
  empty_ = true;
}

bool ExponentialMovingAverage::IsEmpty() const { return empty_; }
