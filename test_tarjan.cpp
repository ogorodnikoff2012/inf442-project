//
// Created by Manas Uteshev on 4/28/2021.
//

#include "tarjan.h"
#include "util.h"

#include <fstream>

int main(int argc, char* argv[]) {
  std::ifstream fin("tarjan.in");
  std::ofstream fout("tarjan.out");

  Graph gr = ReadFromEdgeList(fin);
  fin.close();

  auto components = FindConnectedComponentsTarjan(gr);
  fout << "Components:\n";
  for (const auto& component : components) {
    fout << component << '\n';
  }
  fout.close();

  return 0;
}
