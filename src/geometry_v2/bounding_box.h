//
// Created by xenon on 02.05.2021.
//

#ifndef INF442_BOUNDING_BOX_H
#define INF442_BOUNDING_BOX_H

#include "point.h"

#include <cstddef>
#include <limits>

namespace geometry_v2 {

template <class T>
class BoundingBox {
 public:
  using ValueType = T;

  explicit BoundingBox(size_t dimensionality)
      : lower_(dimensionality, std::numeric_limits<T>::max())
      , upper_(dimensionality, std::numeric_limits<T>::lowest()) {}

  BoundingBox() = default;

  BoundingBox(const BoundingBox&) = default;

  explicit BoundingBox(const Point<T>& point)
      : lower_(point)
      , upper_(point) {}

  BoundingBox(std::initializer_list<Point<T>> points)
      : BoundingBox() {
    for (const auto& point : points) {
      Extend(point);
    }
  }

  template <class... Args>
  BoundingBox(Args&&... args)
      : BoundingBox() {
    (Extend(std::forward<Args>(args)), ...);
  }

  size_t Dimensionality() const { return lower_.Dimensionality(); }

  template <class U>
  void Extend(const Point<U>& point) {
    if (Dimensionality() == 0) {
      lower_ = point;
      upper_ = point;
      return;
    }

    CheckDimensionality(*this, point);

    for (size_t i = 0; i < Dimensionality(); ++i) {
      lower_[i] = std::min<T>(lower_[i], point[i]);
      upper_[i] = std::max<T>(upper_[i], point[i]);
    }
  }

  template <class U>
  void Extend(const BoundingBox<U>& box) {
    Extend(box.Lower());
    Extend(box.Upper());
  }

  bool IsEmpty() const {
    if (Dimensionality() == 0) {
      return true;
    }

    for (size_t i = 0; i < Dimensionality(); ++i) {
      if (lower_[i] > upper_[i]) {
        return true;
      }
    }
    return false;
  }

  operator bool() const { return !IsEmpty(); }

  bool Contains(const Point<T>& point) const {
    CheckDimensionality(*this, point);

    for (size_t i = 0; i < Dimensionality(); ++i) {
      if (point[i] < lower_[i] || upper_[i] < point[i]) {
        return false;
      }
    }
    return true;
  }

  BoundingBox IntersectWith(const BoundingBox& other) const {
    CheckDimensionality(*this, other);

    BoundingBox result(Dimensionality());

    for (size_t i = 0; i < Dimensionality(); ++i) {
      result.lower_[i] = std::max<T>(lower_[i], other.lower_[i]);
      result.upper_[i] = std::min<T>(upper_[i], other.upper_[i]);
    }

    return result;
  }

  const Point<T>& Lower() const { return lower_; }

  const Point<T>& Upper() const { return upper_; }

 private:
  Point<T> lower_;
  Point<T> upper_;
};

} // namespace geometry_v2

#endif // INF442_BOUNDING_BOX_H
