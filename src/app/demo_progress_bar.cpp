//
// Created by xenon on 29.04.2021.
//

#include "../util/progress_bar.h"

#include <chrono>
#include <iostream>
#include <random>
#include <thread>

int main() {
  using namespace std::chrono_literals;

  const int N = 100;
  std::default_random_engine rnd(
      std::chrono::system_clock::now().time_since_epoch().count());
  std::uniform_int_distribution<int> dis(150, 250);
  util::ProgressBar bar(std::cerr);
  bar.SetLimit(N);
  for (int i = 0; i < N; ++i) {
    std::this_thread::sleep_for(std::chrono::milliseconds{dis(rnd)});
    bar.IncrementCounter();
    bar.Out() << i;
  }

  return 0;
}