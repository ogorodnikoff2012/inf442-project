//
// Created by xenon on 02.05.2021.
//

#include "../geometry_v2/pseudo_dbscan.h"
#include "../util/progress_bar.h"
#include "../util/util.h"

#include <cstdio>
#include <iostream>
#include <sstream>

template <class T>
std::vector<T> Tokenize(const std::string& line) {
  std::vector<T> result;
  std::istringstream ss(line);
  T object;

  while ((ss >> object)) {
    result.push_back(object);
  }

  return result;
}

template <class T>
std::vector<geometry_v2::Point<T>> ReadPoints(std::istream& in) {
  size_t cnt = 0;
  if (!(in >> cnt)) {
    throw std::runtime_error("Cannot read cnt");
  }

  util::ProgressBar bar(std::cerr);
  bar.SetLimit(cnt);

  std::vector<geometry_v2::Point<T>> result;
  result.reserve(cnt);

  std::string buffer;
  std::getline(in, buffer);

  for (size_t i = 0; i < cnt; ++i, bar.IncrementCounter()) {
    std::getline(in, buffer);
    std::vector<T> values = Tokenize<T>(buffer);
    result.emplace_back(std::move(values));
  }

  return result;
}

namespace geometry_v2 {

template <class T>
std::ostream& operator<<(std::ostream& out, const geometry_v2::Point<T>& pt) {
  out << '(';
  bool first = true;
  for (size_t i = 0; i < pt.Dimensionality(); ++i) {
    if (!first) {
      out << ", ";
    }
    first = false;
    out << pt[i];
  }
  return out << ')';
}

} // namespace geometry_v2

int main(int argc, char* argv[]) {
  if (argc < 3 || argc > 5) {
    std::cerr << "Usage: " << argv[0]
              << " min_points eps [input_file [output_file]]\n";
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
  auto points = ReadPoints<double>(std::cin);
  std::cerr << "Running DBSCAN\n";
  geometry_v2::PseudoDBSCAN dbscan(points, min_points, eps, true);
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