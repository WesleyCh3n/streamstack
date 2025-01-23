#include "camera.hpp"

#include <opencv2/core/cvstd_wrapper.hpp>
#include <opencv2/cudacodec.hpp>

class Camera::Impl {
  std::string url_;
  cv::Ptr<cv::cudacodec::VideoReader> camera_ = nullptr;
  bool is_open_ = false;

public:
  Impl(const std::string &url) : url_(url) { open(url_); }
  void open(const std::string &url) {
    if (camera_) {
      camera_.release();
    }
    // TODO: do belows nessasary
    setenv("OPENCV_FFMPEG_THREADS", "1", true);
    setenv("OPENCV_FFMPEG_LOGLEVEL", "-8", true);
    setenv("OPENCV_FFMPEF_CAPTURE_OPTIONS", "hwaccel;cuda", true);
    auto params = cv::cudacodec::VideoReaderInitParams{};
    // Allow frames to be dropped when ingesting from a live capture source to
    // prevent delay and eventual disconnection when calls to nextFrame()/grab()
    // cannot keep up with the source's fps
    //
    // https://docs.opencv.org/4.10.0/dd/d7d/structcv_1_1cudacodec_1_1VideoReaderInitParams.html#details
    params.allowFrameDrop = true;
    camera_ = cv::cudacodec::createVideoReader(url, {}, params);
    camera_->set(cv::cudacodec::ColorFormat::BGR); // Capture type to CV_8UC3
    is_open_ = true;
  }
  void retrieve(cv::cuda::GpuMat &frame) {
    camera_->grab();
    camera_->retrieve(frame);
  }
  void retrieve(cv::Mat &frame) {
    cv::cuda::GpuMat gpu_frame;
    camera_->grab();
    camera_->retrieve(gpu_frame);
    gpu_frame.download(frame);
  }
};

Camera::Camera(const std::string &url) : impl_(std::make_unique<Impl>(url)) {}
Camera::~Camera() = default;

void Camera::retrieve(cv::cuda::GpuMat &frame) { impl_->retrieve(frame); };
void Camera::retrieve(cv::Mat &frame) { impl_->retrieve(frame); };
