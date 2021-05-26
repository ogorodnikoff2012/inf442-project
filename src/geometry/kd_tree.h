//
// Created by xenon on 02.05.2021.
//

#ifndef INF442_KD_TREE_H
#define INF442_KD_TREE_H

#include "bounding_box.h"
#include "point.h"

#include <algorithm>
#include <memory>
#include <numeric>
#include <stdexcept>
#include <vector>

namespace geometry {

template <class T, size_t N>
class KDTree {
 public:
  using ValueType                         = T;
  static constexpr size_t kDimensionality = N;

  template <class Iterator>
  KDTree(Iterator begin, Iterator end)
      : points_(begin, end) {
    indices_.resize(points_.size());
    std::iota(indices_.begin(), indices_.end(), 0);
    BuildTree();
  }

  template <class Shape>
  std::vector<size_t> IndicesIn(const Shape& shape) const {
    std::vector<size_t> result;
    ForEachIn(shape, [&](const Point<T, N>& point, size_t index) {
      result.push_back(index);
    });
    return result;
  }

  template <class Shape>
  std::vector<Point<T, N>> PointsIn(const Shape& shape) const {
    std::vector<Point<T, N>> result;
    ForEachIn(shape, [&](const Point<T, N>& point, size_t index) {
      result.push_back(point);
    });
    return result;
  }

  template <class Shape, class Callback>
  void ForEachIn(const Shape& shape, Callback callback) const {
    ForEachIn(shape.GetBoundingBox(), [&](const Point<T, N>& point, size_t index) {
      if (shape.Contains(point)) {
        callback(point, index);
      }
    });
  }

  template <class Callback>
  void ForEachIn(const BoundingBox<T, N>& box, Callback callback) const {
    ForEachInHelper(root_, box, callback);
  }

  const Point<T, N>& NearestPoint(const Point<T, N>& pt, size_t* index = nullptr) const {
    if (!root_) {
      throw std::runtime_error("Tree is empty");
    }

    size_t best_index = GetIndex(root_);
    T best_dist_sqr   = pt.DistanceSqr(points_[best_index]);
    NearestPointHelper(root_, pt, &best_index, &best_dist_sqr);
    if (index) {
      *index = best_index;
    }

    return points_[best_index];
  }

  const std::vector<Point<T, N>>& Points() const { return points_; }

 private:
  using IndexIterator = std::vector<size_t>::iterator;
  struct Node {
    size_t dimension;

    IndexIterator index_begin;
    IndexIterator index_end;

    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;

    BoundingBox<T, N> bounding_box;

    Node(size_t dimension, IndexIterator index_begin, IndexIterator index_end,
         std::unique_ptr<Node>&& left, std::unique_ptr<Node>&& right,
         const BoundingBox<T, N>& box)
        : dimension(dimension)
        , index_begin(index_begin)
        , index_end(index_end)
        , left(std::move(left))
        , right(std::move(right))
        , bounding_box(box) {}
  };

  std::vector<Point<T, N>> points_;
  std::vector<size_t> indices_;
  std::unique_ptr<Node> root_{nullptr};

  void BuildTree() {
    root_ = BuildTreeHelper(indices_.begin(), indices_.end(), 0);
  }

  static IndexIterator Middle(IndexIterator begin, IndexIterator end) {
    return begin + std::distance(begin, end) / 2;
  }

  size_t GetIndex(const std::unique_ptr<Node>& node) const {
    return *Middle(node->index_begin, node->index_end);
  }

  void NearestPointHelper(const std::unique_ptr<Node>& node, const Point<T, N>& pt,
                          size_t* best_index, T* best_dist_sqr) const {
    if (!node) {
      return;
    }

    size_t index = GetIndex(node);
    if (index != *best_index) {
      T dist_sqr = pt.DistanceSqr(points_[index]);
      if (dist_sqr < *best_dist_sqr) {
        *best_index    = index;
        *best_dist_sqr = dist_sqr;
      }
    }

    if (*best_dist_sqr == 0) {
      return;
    }

    T delta = pt[node->dimension] - points_[index][node->dimension];
    NearestPointHelper(delta > 0 ? node->right : node->left, pt, best_index,
                       best_dist_sqr);
    if (delta * delta <= *best_dist_sqr) {
      NearestPointHelper(delta > 0 ? node->left : node->right, pt, best_index,
                         best_dist_sqr);
    }
  }

  std::unique_ptr<Node> BuildTreeHelper(IndexIterator begin, IndexIterator end,
                                        size_t dimension) {
    if (begin == end) {
      return nullptr;
    }

    IndexIterator middle = Middle(begin, end);

    std::nth_element(begin, middle, end, [&](size_t lhs, size_t rhs) {
      return points_[lhs][dimension] < points_[rhs][dimension];
    });

    size_t next_dimension = (dimension + 1) % N;
    auto left             = BuildTreeHelper(begin, middle, next_dimension);
    auto right            = BuildTreeHelper(middle + 1, end, next_dimension);

    BoundingBox<T, N> box(points_[*middle]);
    if (left) {
      box.Extend(left->bounding_box);
    }
    if (right) {
      box.Extend(right->bounding_box);
    }

    return std::make_unique<Node>(dimension, begin, end, std::move(left),
                                  std::move(right), box);
  }

  template <class Callback>
  void ForEachInHelper(const std::unique_ptr<Node>& node,
                       const BoundingBox<T, N>& box, Callback callback) const {
    if (!node) {
      return;
    }
    if (!box.IntersectWith(node->bounding_box)) {
      return;
    }

    size_t index      = GetIndex(node);
    const auto& point = points_[index];
    if (box.Contains(point)) {
      callback(point, index);
    }

    ForEachInHelper(node->left, box, callback);
    ForEachInHelper(node->right, box, callback);
  }
};

} // namespace geometry

#endif // INF442_KD_TREE_H
