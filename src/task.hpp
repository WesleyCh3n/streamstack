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
  I request_;
  //   std::promise<O> output_promise;
  //
public:
  void set_request(I &&request) { request_ = std::move(request); }
  [[__nodiscard__]] I get_request() { return std::move(request_); }

  void set_response(O &&response) {}
  [[__nodiscard__]] O get_response() {}
  // void set_response(O const &response) {}
};
