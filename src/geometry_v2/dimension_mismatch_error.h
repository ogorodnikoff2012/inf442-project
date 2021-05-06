//
// Created by xenon on 05.05.2021.
//

#pragma once

#include <stdexcept>

namespace geometry_v2 {

class DimensionMismatchError : public std::runtime_error {
 public:
  DimensionMismatchError(size_t expected, size_t actual);
  size_t GetExpectedDimension() const;
  size_t GetActualDimension() const;

 private:
  size_t expected_;
  size_t actual_;
};

} // namespace geometry_v2