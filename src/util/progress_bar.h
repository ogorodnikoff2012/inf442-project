//
// Created by xenon on 29.04.2021.
//

#ifndef INF442_PROGRESS_BAR_H
#define INF442_PROGRESS_BAR_H

#include "../stats/statistics.h"

#include <chrono>
#include <ostream>

namespace util {

class ProgressBar {
 private:
  class OutputWrapper {
   public:
    explicit OutputWrapper(std::ostream& out, std::string_view replacement);
    ~OutputWrapper();

    template <class T>
    OutputWrapper& operator<<(T&& value) {
      out_ << std::forward<T>(value);
      return *this;
    }

   private:
    std::ostream& out_;
    std::string_view replacement_;
  };

 public:
  explicit ProgressBar(std::ostream& out);
  ~ProgressBar();
  void SetLimit(size_t limit);
  void SetCounter(size_t counter);
  void IncrementCounter();
  void ResetCounter();
  void ForceUpdate();

  OutputWrapper Out();

 private:
  char buffer_[80]{};
  size_t limit_{1};
  size_t counter_{0};
  std::chrono::time_point<std::chrono::system_clock> last_update_time_;
  std::chrono::time_point<std::chrono::system_clock> last_render_time_{};
  stats::ExponentialMovingAverage average_duration_;
  std::ostream& out_;
  bool force_update_{false};

  void Render();
  void ResetDuration();
  void UpdateDuration();
};

} // namespace util

#endif // INF442_PROGRESS_BAR_H
