//
// Created by xenon on 29.04.2021.
//

#include "progress_bar.h"

#include <cstring>

ProgressBar::ProgressBar(std::ostream& out)
    : last_update_time_(std::chrono::system_clock::now())
    , average_duration_(2, 50)
    , out_{out} {
  std::fill_n(buffer_, sizeof(buffer_), '\0');
  Render();
}

void ProgressBar::SetLimit(size_t limit) {
  limit_ = limit;
  ResetDuration();
  Render();
}

void ProgressBar::SetCounter(size_t counter) {
  counter_ = counter;
  ResetDuration();
  Render();
}

void ProgressBar::IncrementCounter() {
  ++counter_;
  UpdateDuration();
  Render();
}

void ProgressBar::ResetCounter() {
  counter_ = 0;
  ResetDuration();
  Render();
}
ProgressBar::OutputWrapper ProgressBar::Out() {
  return OutputWrapper(out_, std::string_view{buffer_});
}

void ProgressBar::Render() {
  auto now = std::chrono::system_clock::now();

  std::chrono::duration<double> diff = now - last_render_time_;
  if (diff.count() < 0.1 && !force_update_) {
    return;
  } else {
    last_render_time_ = now;
  }
  force_update_ = false;

  for (size_t i = 0, n = std::strlen(buffer_); i < n; ++i) {
    buffer_[i] = ' ';
  }
  out_ << '\r' << buffer_;
  buffer_[0] = '\0';

  char* buf_begin = buffer_;
  char* buf_end   = buffer_ + sizeof(buffer_);

  const size_t percentage = std::min<size_t>(
      100, std::max<size_t>(
               0, static_cast<size_t>(std::round(100.0 * counter_ / limit_))));
  const size_t decades   = percentage / 10;
  const size_t remainder = percentage % 10;

  *buf_begin++ = '[';
  for (size_t i = 0; i < 10; ++i) {
    if (i < decades) {
      *buf_begin++ = '#';
    } else if (i > decades || remainder == 0) {
      *buf_begin++ = ' ';
    } else {
      *buf_begin++ = '0' + remainder;
    }
  }
  *buf_begin++ = ']';

  buf_begin += std::snprintf(buf_begin, buf_end - buf_begin, " (%lu/%lu)",
                             counter_, limit_);

  if (!average_duration_.IsEmpty()) {
    const double avg_dur = average_duration_.Mean();

    if (avg_dur > 1) {
      buf_begin += std::snprintf(buf_begin, buf_end - buf_begin, " %.2f s/it.,",
                                 avg_dur);
    } else {
      buf_begin += std::snprintf(buf_begin, buf_end - buf_begin, " %.2f it./s,",
                                 1 / avg_dur);
    }

    const size_t eta = 1000 * avg_dur * (limit_ - counter_);
    // const size_t eta_ms = eta % 1000;
    const size_t eta_ss = (eta / 1000) % 60;
    const size_t eta_mm = (eta / 60'000) % 60;
    const size_t eta_hh = eta / 3'600'000;

    /*buf_begin += */ std::snprintf(buf_begin, buf_end - buf_begin,
                                    " ETA %02lu:%02lu:%02lu"
                                    /* ".%03lu" */
                                    ,
                                    eta_hh, eta_mm, eta_ss
                                    /*, eta_ms */);
  } else {
    /*buf_begin += */ std::snprintf(buf_begin, buf_end - buf_begin, " ETA N/A");
  }

  out_ << '\r' << buffer_;
  out_.flush();
}

void ProgressBar::ResetDuration() {
  last_update_time_ = std::chrono::system_clock::now();
  average_duration_.Reset();
}

void ProgressBar::UpdateDuration() {
  auto now                           = std::chrono::system_clock::now();
  std::chrono::duration<double> diff = now - last_update_time_;
  last_update_time_                  = now;
  average_duration_.Add(diff.count());
}

ProgressBar::~ProgressBar() {
  force_update_ = true;
  Render();
  out_ << '\n';
}

ProgressBar::OutputWrapper::OutputWrapper(std::ostream& out,
                                          std::string_view replacement)
    : out_(out)
    , replacement_(replacement) {
  out_ << '\r';
  for (size_t i = 0; i < replacement_.size(); ++i) {
    out_ << ' ';
  }
  out_ << '\r';
}

ProgressBar::OutputWrapper::~OutputWrapper() {
  out_ << '\n' << replacement_;
  out_.flush();
}
