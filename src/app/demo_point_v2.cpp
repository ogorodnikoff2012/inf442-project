//
// Created by xenon on 30.04.2021.
//

#include "../geometry_v2/kd_tree.h"
#include "../geometry_v2/point.h"
#include "../geometry_v2/sphere.h"

#include <cmath>
#include <iostream>
#include <vector>

using Point2D = geometry_v2::Point<double>;
using Point2I = geometry_v2::Point<int>;

void PrintPoint(const Point2D& point) {
  std::cout << point[0] << " " << point[1] << std::endl;
}

int main() {
  Point2D pt(2);

  pt[0] = 5;
  pt[1] = 6;
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
  //   geometry::KDTree<double, 2> tree(points.begin(), points.end());
  //
  //   for (double radius : {1., std::sqrt(2), 2. + 1e-6}) {
  //     std::cout << "Sphere with center=(2, 2), radius=" << radius <<
  //     std::endl; tree.ForEachIn(geometry::Sphere{Point2D{2., 2.}, radius},
  //                    [](const auto& point, size_t index) { PrintPoint(point);
  //                    });
  //   }
  //
  return 0;
}