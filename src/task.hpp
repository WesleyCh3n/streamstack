#pragma once

#include "noncopyable.hpp"

#include <future>
#include <opencv2/core/mat.hpp>

class DetectionTask {
  std::promise<std::vector<cv::Mat>> promise;

public:
  cv::Mat mat;
  std::future<std::vector<cv::Mat>> get_future() {
    return promise.get_future();
  };
  void set_value(std::vector<cv::Mat> &&value) { promise.set_value(value); }
};
