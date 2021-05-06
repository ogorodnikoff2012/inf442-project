//
// Created by Manas Uteshev on 4/28/2021.
//

#include "../graph/tarjan.h"
#include "../util/progress_bar.h"
#include "../util/util.h"

#include <fstream>

int main(int argc, char* argv[]) {
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " input_file output_file\n";
    return 1;
  }

  std::ifstream fin(argv[1]);
  std::ofstream fout(argv[2]);

  std::cerr << "Reading\n";
  graph::Graph gr = graph::ReadFromEdgeList(fin);
  std::cerr << "Reading done\nProcessing\n";
  fin.close();

  auto components = graph::FindConnectedComponentsTarjan(gr, true);
  std::cerr << "Processing done\nSaving\n";

  {
    util::ProgressBar bar(std::cerr);
    bar.SetLimit(components.size() + 1);

    fout << "# Components:\ncomponents = [\n";
    for (const auto& component : components) {
      fout << "  " << component << ",\n";
      bar.IncrementCounter();
    }
    fout << "]\n";

    fout << "# Size distributions:\nsizes = ";
    std::map<size_t, size_t> size_counts;
    for (const auto& component : components) {
      ++size_counts[component.size()];
    }
    fout << size_counts << '\n';
    bar.IncrementCounter();
    fout.close();
  }

  std::cerr << "Done\n";

  return 0;
}
