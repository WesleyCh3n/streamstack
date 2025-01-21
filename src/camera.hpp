#pragma once
#include <memory>
#include <string>

#include <opencv2/core/cuda.hpp>

class Camera {
  class Impl;
  std::unique_ptr<Impl> impl_;

public:
  Camera(const std::string &url);
  ~Camera();

  void retrieve(cv::cuda::GpuMat &frame);
};
