//
// Created by xenon on 02.05.2021.
//

#ifndef INF442_SPHERE_H
#define INF442_SPHERE_H

#include "bounding_box.h"
#include "point.h"

namespace geometry {

template <class T, size_t N>
class Sphere {
 public:
  using ValueType                         = T;
  static constexpr size_t kDimensionality = N;

  Sphere(const Point<T, N>& center, const T& radius)
      : center_(center)
      , radius_(radius)
      , radius_sqr_(radius * radius) {}

  Sphere(const Sphere&) = default;

  bool Contains(const Point<T, N>& point) const {
    return point.DistanceSqr(center_) <= radius_sqr_;
  }

  BoundingBox<T, N> GetBoundingBox() const {
    Point<T, N> lower = center_;
    Point<T, N> upper = center_;

#pragma unroll
    for (size_t i = 0; i < N; ++i) {
      lower[i] -= radius_;
      upper[i] += radius_;
    }

    return BoundingBox<T, N>(lower, upper);
  }

 private:
  Point<T, N> center_;
  T radius_;
  T radius_sqr_;
};

} // namespace geometry

#endif // INF442_SPHERE_H
