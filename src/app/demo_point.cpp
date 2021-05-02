//
// Created by xenon on 30.04.2021.
//

#include "../geometry/kd_tree.h"
#include "../geometry/point.h"
#include "../geometry/sphere.h"

#include <cmath>
#include <iostream>
#include <vector>

using Point2D = Point<double, 2>;
using Point2I = Point<int, 2>;

void PrintPoint(const Point2D& point) {
  std::cout << point.X() << " " << point.Y() << std::endl;
}

int main() {
  Point2D pt;

  pt.X() = 5;
  pt.Y() = 6;
  // pt.Z() = 7; // error

  Point2I pti{2, 2};

  std::cout << pt.DistanceSqr(pti) << std::endl;

  pt = pti;

  std::cout << pt.DistanceSqr(pti) << std::endl;

  std::vector<Point2D> points{{1., 1.},
                              {2., 2.},
                              {3., 3.},
                              {1., 2.},
                              {2 - std::sqrt(2), 2 - std::sqrt(2)}};
  KDTree<double, 2> tree(points.begin(), points.end());

  for (double radius : {1., std::sqrt(2), 2. + 1e-6}) {
    std::cout << "Sphere with center=(2, 2), radius=" << radius << std::endl;
    tree.ForEachIn(Sphere{Point2D{2., 2.}, radius},
                   [](const auto& point, size_t index) { PrintPoint(point); });
  }

  return 0;
}