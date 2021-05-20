//
// Created by xenon on 19.05.2021.
//

#ifndef INF442_PSEUDO_DBSCAN_H
#define INF442_PSEUDO_DBSCAN_H

#include "kd_tree.h"
#include "point.h"
#include "sphere.h"

#include "../graph/graph.h"
#include "../graph/kosaraju.h"
#include "../util/progress_bar.h"

#include <iostream>
#include <unordered_set>

namespace geometry_v2 {

template <class T>
class PseudoDBSCAN {
 public:
  using ValueType = T;

  explicit PseudoDBSCAN(const std::vector<Point<T>>& points, size_t min_points,
                        const T& eps, bool show_progress = false)
      : tree_(points.begin(), points.end())
      , min_points_(min_points)
      , eps_(eps)
      , point_to_cluster_(points.size(), kLabelUndefined) {
    Fit(points, show_progress);
  }

  size_t Dimensionality() const { return tree_->Dimensionality(); }

  const std::vector<int>& PointToCluster() const { return point_to_cluster_; }
  const std::vector<std::vector<size_t>>& Clusters() const { return clusters_; }
  const std::vector<size_t>& Outliers() const { return outliers_; }

  std::vector<std::vector<Point<T>>> ClustersAsPoints() const {
    std::vector<std::vector<Point<T>>> result(clusters_.size());

    for (size_t i = 0; i < clusters_.size(); ++i) {
      result[i].resize(clusters_[i].size());
      for (size_t j = 0; j < clusters_[i].size(); ++j) {
        result[i][j] = tree_.Points()[clusters_[i][j]];
      }
    }

    return result;
  }

  std::vector<Point<T>> OutliersAsPoints() const {
    std::vector<Point<T>> result(outliers_.size());

    for (size_t i = 0; i < outliers_.size(); ++i) {
      result[i] = tree_.Points()[outliers_[i]];
    }

    return result;
  }

 private:
  static constexpr int kLabelUndefined = -2;
  static constexpr int kLabelOutlier   = -1;

  const KDTree<T> tree_;
  const size_t min_points_;
  const T eps_;

  std::vector<int> point_to_cluster_;
  std::vector<std::vector<size_t>> clusters_{};
  std::vector<size_t> outliers_{};

  void FillClustersAndOutliers() {
    for (size_t i = 0; i < point_to_cluster_.size(); ++i) {
      int label = point_to_cluster_[i];
      if (label == kLabelOutlier) {
        outliers_.push_back(i);
      } else {
        clusters_[label].push_back(i);
      }
    }
  }

  static inline void TryIncrement(std::unique_ptr<util::ProgressBar>& bar) {
    if (bar) {
      bar->IncrementCounter();
    }
  }

  void Fit(const std::vector<Point<T>>& points, bool show_progress) {
    graph::Graph gr(points.size());

    std::unique_ptr<util::ProgressBar> bar{nullptr};
    if (show_progress) {
      bar.reset(new util::ProgressBar(std::cerr));
      bar->SetLimit(points.size());
      bar->ForceUpdate();
    }

    for (size_t i = 0; i < points.size(); ++i, TryIncrement(bar)) {
      auto neighbours = tree_.IndicesIn(Sphere<T>(points[i], eps_));
      if (neighbours.size() >= min_points_) {
        for (size_t j : neighbours) {
          gr.AddEdge(i, j);
        }
      }
    }
    bar.reset();

    std::vector<int64_t> input_edges(points.size(), -1);
    for (size_t v = 0; v < gr.VertexCount(); ++v) {
      for (size_t u : gr.GetEdges(v)) {
        input_edges[u] = v;
      }
    }

    std::fill(point_to_cluster_.begin(), point_to_cluster_.end(),
              kLabelOutlier);
    size_t cluster_counter = 0;
    auto components = graph::FindConnectedComponentsKosaraju(gr, show_progress);

    for (const auto& component : components) {
      if (component.size() > 1) {
        size_t cluster_id = cluster_counter++;
        for (size_t v : component) {
          point_to_cluster_[v] = cluster_id;
        }
        continue;
      }

      size_t element = component.front();
      if (!gr.GetEdges(element).empty()) {
        point_to_cluster_[element] = cluster_counter++;
      } else if (input_edges[element] >= 0) {
        point_to_cluster_[element] = point_to_cluster_[input_edges[element]];
      }
    }

    clusters_.resize(cluster_counter);
    FillClustersAndOutliers();
  }
};

} // namespace geometry_v2

#endif // INF442_PSEUDO_DBSCAN_H
