#pragma once

#include <future>
#include <opencv2/core/mat.hpp>

using Data = std::vector<cv::Mat>;
class DetectionTask {
  std::promise<Data> promise;

public:
  cv::Mat mat;
  void set_value(Data &&value) { promise.set_value(std::move(value)); }
  std::future<Data> get_future() { return promise.get_future(); };
};

template <typename I, typename O> class Task {
  I input_;
  //   std::promise<O> output_promise;
  //
public:
  Task(I &&input) : input_(std::move(input)) {}
  [[__nodiscard__]] I get_input() { return std::move(input_); }
};
