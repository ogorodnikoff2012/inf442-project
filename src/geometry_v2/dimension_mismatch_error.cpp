//
// Created by xenon on 05.05.2021.
//
#include "dimension_mismatch_error.h"

#include <sstream>

namespace {

std::string DimensionMismatchErrorWhat(size_t expected, size_t actual) {
  std::stringstream ss;
  ss << "Dimension mismatch (expected: " << expected << ", actual: " << actual
     << ')';

  return ss.str();
}

} // namespace

namespace geometry_v2 {

DimensionMismatchError::DimensionMismatchError(size_t expected, size_t actual)
    : runtime_error(DimensionMismatchErrorWhat(expected, actual))
    , expected_(expected)
    , actual_(actual) {}

size_t DimensionMismatchError::GetExpectedDimension() const {
  return expected_;
}
size_t DimensionMismatchError::GetActualDimension() const { return actual_; }

} // namespace geometry_v2