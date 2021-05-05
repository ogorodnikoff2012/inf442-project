//
// Created by xenon on 02.05.2021.
//

#include "../geometry/dbscan.h"
#include "../util/progress_bar.h"
#include "../util/util.h"

#include <cstdio>
#include <iostream>
#include <sstream>

template <class T, size_t N>
std::vector<Point<T, N>> ReadPoints(std::istream& in) {
  size_t cnt = 0;
  if (!(in >> cnt)) {
    throw std::runtime_error("Cannot read cnt");
  }

  ProgressBar bar(std::cerr);
  bar.SetLimit(cnt);

  std::vector<Point<T, N>> result(cnt);
  for (size_t i = 0; i < cnt; ++i, bar.IncrementCounter()) {
    for (size_t j = 0; j < N; ++j) {
      if (!(in >> result[i][j])) {
        std::stringstream ss;
        ss << "Cannot read coordinate " << j << " of point " << i;
        throw std::runtime_error(ss.str());
      }
    }
  }

  return result;
}

template <class T, size_t N>
std::ostream& operator<<(std::ostream& out, const Point<T, N>& pt) {
  const char* prefix = "(";
  for (size_t i = 0; i < N; ++i) {
    out << prefix << pt[i];
    prefix = ", ";
  }
  return out << ')';
}

int main(int argc, char* argv[]) {
  if (argc < 3 || argc > 5) {
    std::cerr << "Usage: " << argv[0] << " min_points eps [input_file [output_file]]\n";
    return 1;
  }

  const size_t min_points = std::strtoull(argv[1], nullptr, 10);
  const double eps        = std::strtod(argv[2], nullptr);

  if (argc >= 4) {
    std::freopen(argv[3], "r", stdin);
    if (argc >= 5) {
      std::freopen(argv[4], "w", stdout);
    }
  }

  std::cerr << "Reading\n";
  auto points = ReadPoints<double, 2>(std::cin);
  std::cerr << "Running DBSCAN\n";
  DBSCAN dbscan(points, min_points, eps, true);
  std::cerr << "Done\n";

  const auto& clusters = dbscan.Clusters();
  const auto& outliers = dbscan.Outliers();

  std::cout << "# Number of clusters\nnum_of_clusters = " << clusters.size()
            << '\n';
  std::cout << "# Indices of cluster\nclusters = [\n";
  for (const auto& cluster : clusters) {
    std::cout << "    " << cluster << ",\n";
  }
  std::cout << "]\n# Indices of outliers\noutliers = " << outliers
            << "\n# Points\npoints = " << points << '\n';

  return 0;
}