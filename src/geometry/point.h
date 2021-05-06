//
// Created by xenon on 30.04.2021.
//

#ifndef INF442_POINT_H
#define INF442_POINT_H

#include <array>
#include <cstddef>
#include <type_traits>

namespace geometry {

template <class T, size_t N>
class Point {
 public:
  using ValueType                         = T;
  static constexpr size_t kDimensionality = N;

  Point()             = default;
  Point(const Point&) = default;
  Point(Point&)       = default;

  template <class U>
  Point(const Point<U, N>& other) {
    for (size_t i = 0; i < N; ++i) {
      data_[i] = other[i];
    }
  }

  template <class U>
  Point& operator=(const Point<U, N>& other) {
    for (size_t i = 0; i < N; ++i) {
      data_[i] = other[i];
    }
    return *this;
  }

  bool operator==(const Point& other) const {
#pragma unroll
    for (size_t i = 0; i < N; ++i) {
      if (data_[i] != other.data_[i]) {
        return false;
      }
    }
    return true;
  }

  bool operator!=(const Point& other) const { return !(*this == other); }

  Point(const std::array<T, N>& data)
      : data_(data) {}

  template <class... Args>
  Point(Args&&... args)
      : data_{std::forward<Args>(args)...} {}

  Point& operator=(const Point&) = default;

#define CONVENTIONAL_COORDINATE_NAME(name, index)                              \
  template <size_t n = N, size_t i = (index)>                                  \
  std::enable_if_t<(n == N) && (i == (index)) && (n > i), T&> name() {         \
    return data_[i];                                                           \
  }                                                                            \
  template <size_t n = N, size_t i = (index)>                                  \
  std::enable_if_t<(n == N) && (i == (index)) && (n > i), const T&> name()     \
      const {                                                                  \
    return data_[i];                                                           \
  }

  CONVENTIONAL_COORDINATE_NAME(X, 0)
  CONVENTIONAL_COORDINATE_NAME(Y, 1)
  CONVENTIONAL_COORDINATE_NAME(Z, 2)
#undef CONVENTIONAL_COORDINATE_NAME

  T& operator[](size_t index) { return data_[index]; }

  const T& operator[](size_t index) const { return data_[index]; }

  template <class U>
  auto DistanceSqr(const Point<U, N>& other) const {
    using ResultType = std::common_type_t<T, U>;
    ResultType result{};
#pragma unroll
    for (size_t i = 0; i < N; ++i) {
      auto delta = data_[i] - other[i];
      result += delta * delta;
    }
    return result;
  }

 private:
  std::array<T, N> data_{};
};

} // namespace geometry

#endif // INF442_POINT_H
