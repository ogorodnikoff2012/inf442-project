//
// Created by xenon on 02.05.2021.
//

#ifndef INF442_SPHERE_H
#define INF442_SPHERE_H

#include "bounding_box.h"
#include "point.h"

namespace geometry_v2 {

template <class T>
class Sphere {
 public:
  using ValueType = T;

  Sphere(const Point<T>& center, const T& radius)
      : center_(center)
      , radius_(radius)
      , radius_sqr_(radius * radius) {}

  Sphere(const Sphere&) = default;

  bool Contains(const Point<T>& point) const {
    return point.DistanceSqr(center_) <= radius_sqr_;
  }

  size_t Dimensionality() const { return center_.Dimensionality(); }

  BoundingBox<T> GetBoundingBox() const {
    Point<T> lower = center_;
    Point<T> upper = center_;

    for (size_t i = 0; i < Dimensionality(); ++i) {
      lower[i] -= radius_;
      upper[i] += radius_;
    }

    return BoundingBox<T>(lower, upper);
  }

 private:
  Point<T> center_;
  T radius_;
  T radius_sqr_;
};

} // namespace geometry_v2

#endif // INF442_SPHERE_H
