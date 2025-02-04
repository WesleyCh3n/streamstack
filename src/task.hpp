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

template <typename Data, typename ResponseType> class Task {
  Data data_;

public:
  explicit Task() = delete;

  template <typename... Args>
  explicit Task(Args &&...data) : data_(std::forward<Data>(data)...) {}

  void take(Data &item) {
    item = std::move(data_); // prevent copy
  }

  void set_response(ResponseType &&response) {
    response_promise_.set_value(std::forward<ResponseType>(response));
  }
  [[__nodiscard__]] std::future<ResponseType> get_response() {
    return response_promise_.get_future();
  }

private:
  std::promise<ResponseType> response_promise_;
};
