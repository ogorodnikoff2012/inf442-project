//
// Created by xenon on 30.04.2021.
//

#ifndef INF442_POINT_H
#define INF442_POINT_H

#include "dimension_mismatch_error.h"

#include <cstddef>
#include <type_traits>
#include <vector>

namespace geometry_v2 {

template <class T, class U>
void CheckDimensionality(const T& lhs, const U& rhs) {
  size_t expected = lhs.Dimensionality();
  size_t actual   = rhs.Dimensionality();
  if (expected != actual) {
    throw DimensionMismatchError(expected, actual);
  }
}

template <class T>
class Point {
 public:
  using ValueType = T;

  Point()             = default;
  Point(const Point&) = default;
  Point(Point&&)      = default;
  Point& operator=(const Point&) = default;
  Point& operator=(Point&&) = default;

  explicit Point(size_t dimension, const T& value = T{})
      : data_(dimension, value) {}

  template <class U>
  Point(std::initializer_list<U> values)
      : data_{values} {}

  Point(const std::vector<T>& values)
      : data_(values) {}
  Point(std::vector<T>&& values)
      : data_(std::move(values)) {}

  template <class U>
  Point(const Point<U>& other) {
    for (size_t i = 0; i < other.Dimensionality(); ++i) {
      data_[i] = other[i];
    }
  }

  template <class U>
  Point& operator=(const Point<U>& other) {
    data_.resize(other.Dimensionality());
    for (size_t i = 0; i < other.Dimensionality(); ++i) {
      data_[i] = other[i];
    }
    return *this;
  }

  size_t Dimensionality() const { return data_.size(); }

  bool operator==(const Point& other) const {
    CheckDimensionality(*this, other);

    for (size_t i = 0; i < Dimensionality(); ++i) {
      if (data_[i] != other.data_[i]) {
        return false;
      }
    }
    return true;
  }

  bool operator!=(const Point& other) const { return !(*this == other); }

  T& operator[](size_t index) { return data_[index]; }

  const T& operator[](size_t index) const { return data_[index]; }

  template <class U>
  auto DistanceSqr(const Point<U>& other) const {
    using ResultType = std::common_type_t<T, U>;

    CheckDimensionality(*this, other);

    ResultType result{};
    for (size_t i = 0; i < Dimensionality(); ++i) {
      auto delta = data_[i] - other[i];
      result += delta * delta;
    }
    return result;
  }

 private:
  std::vector<T> data_{};
};

} // namespace geometry_v2

#endif // INF442_POINT_H
