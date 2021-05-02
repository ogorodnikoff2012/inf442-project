//
// Created by xenon on 02.05.2021.
//

#ifndef INF442_BOUNDING_BOX_H
#define INF442_BOUNDING_BOX_H

#include "point.h"

#include <cstddef>
#include <limits>

template <class T, size_t N>
class BoundingBox {
 public:
  using ValueType                         = T;
  static constexpr size_t kDimensionality = N;

  BoundingBox() {
#pragma unroll
    for (size_t i = 0; i < N; ++i) {
      lower_[i] = std::numeric_limits<T>::max();
      upper_[i] = std::numeric_limits<T>::lowest();
    }
  }

  BoundingBox(const BoundingBox&) = default;

  explicit BoundingBox(const Point<T, N>& point)
      : lower_(point)
      , upper_(point) {}

  BoundingBox(std::initializer_list<Point<T, N>> points)
      : BoundingBox() {
    for (const auto& point : points) {
      Extend(point);
    }
  }

  template <class... Args>
  explicit BoundingBox(Args&&... args)
      : BoundingBox() {
    (Extend(std::forward<Args>(args)), ...);
  }

  template <class U>
  void Extend(const Point<U, N>& point) {
#pragma unroll
    for (size_t i = 0; i < N; ++i) {
      lower_[i] = std::min<T>(lower_[i], point[i]);
      upper_[i] = std::max<T>(upper_[i], point[i]);
    }
  }

  template <class U>
  void Extend(const BoundingBox<U, N>& box) {
    Extend(box.Lower());
    Extend(box.Upper());
  }

  bool IsEmpty() const {
#pragma unroll
    for (size_t i = 0; i < N; ++i) {
      if (lower_[i] > upper_[i]) {
        return true;
      }
    }
    return false;
  }

  operator bool() const { return !IsEmpty(); }

  bool Contains(const Point<T, N>& point) const {
#pragma unroll
    for (size_t i = 0; i < N; ++i) {
      if (point[i] < lower_[i] || upper_[i] < point[i]) {
        return false;
      }
    }
    return true;
  }

  BoundingBox IntersectWith(const BoundingBox& other) const {
    BoundingBox result;

#pragma unroll
    for (size_t i = 0; i < N; ++i) {
      result.lower_[i] = std::max<T>(lower_[i], other.lower_[i]);
      result.upper_[i] = std::min<T>(upper_[i], other.upper_[i]);
    }

    return result;
  }

  const Point<T, N>& Lower() const { return lower_; }

  const Point<T, N>& Upper() const { return upper_; }

 private:
  Point<T, N> lower_;
  Point<T, N> upper_;
};

#endif // INF442_BOUNDING_BOX_H
