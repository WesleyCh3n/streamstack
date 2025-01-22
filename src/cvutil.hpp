#pragma once

#include <opencv2/videoio.hpp>

#include <span>

namespace util {

inline void create_mp4(const std::span<cv::Mat> &frames,
                       const std::string &output_path, const double fps = 30) {
  if (frames.empty()) {
    throw std::runtime_error("Error: No frames provided to create video.");
  }

  cv::Size size(frames[0].cols, frames[0].rows);
  if (size.empty()) {
    throw std::runtime_error("Error: frames with empty size");
  }

  auto fourcc = cv::VideoWriter::fourcc('m', 'p', '4', 'v');
  cv::VideoWriter writer(output_path, fourcc, fps, size);

  if (!writer.isOpened()) {
    throw std::runtime_error(
        "Error: Could not open the video file for writing.");
  }

  // Write each frame to the video
  for (const auto &frame : frames) {
    if (frame.size() != size) {
      throw std::runtime_error("Error: All frames must have the same size.");
    }

    writer << frame;
  }
}

} // namespace util
