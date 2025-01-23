#pragma once

#include <memory>
#include <stdexcept>

#include "datapipe.hpp"

class Streamer {
public:
  virtual ~Streamer() = default;
  virtual void stop() = 0;
  virtual bool is_running() = 0;
};

struct StreamerConfig {
  StreamerConfig &&build() { throw std::logic_error("not implemented"); }
};

// class GPUStreamerThread : public Streamer {
//   class Impl;
//   std::unique_ptr<Impl> impl_;
//
// public:
//   GPUStreamerThread(StreamerConfig, Datapipe *);
//   GPUStreamerThread(GPUStreamerThread &&) noexcept;
//   ~GPUStreamerThread() noexcept;
//   void stop() override;
//   bool is_running() override;
// };

// #include <condition_variable>
// #include <deque>
// #include <mutex>
// #include <opencv2/opencv.hpp>
// #include <thread>
//
// class RTSPCapture {
// public:
//   RTSPCapture(const std::string &stream_url, size_t buffer_size = 5)
//       : stream_url(stream_url), buffer_size(buffer_size), stop_thread(false)
//       {
//     // Start the capture thread
//     capture_thread = std::thread(&RTSPCapture::captureLoop, this);
//   }
//
//   ~RTSPCapture() {
//     stop_thread = true;
//     if (capture_thread.joinable()) {
//       capture_thread.join();
//     }
//   }
//
//   cv::Mat grab_frame() {
//     std::unique_lock<std::mutex> lock(frame_mutex);
//     if (frame_buffer.empty()) {
//       frame_cv.wait(lock,
//                     [this] { return !frame_buffer.empty() || stop_thread; });
//     }
//
//     if (!frame_buffer.empty()) {
//       cv::Mat frame = frame_buffer.front().clone();
//       frame_buffer.pop_front();
//       return frame;
//     }
//     return cv::Mat(); // Return empty Mat if no frame is available
//   }
//
// private:
//   void captureLoop() {
//     cv::VideoCapture cap(stream_url);
//     if (!cap.isOpened()) {
//       throw std::runtime_error("Failed to open the RTSP stream");
//     }
//
//     while (!stop_thread) {
//       cv::Mat frame;
//       if (cap.read(frame)) {
//         std::lock_guard<std::mutex> lock(frame_mutex);
//         if (frame_buffer.size() >= buffer_size) {
//           frame_buffer.pop_front(); // Remove the oldest frame if buffer is
//           full
//         }
//         frame_buffer.push_back(frame);
//         frame_cv.notify_one(); // Notify one waiting thread
//       } else {
//         // Handle read errors or stream disconnection
//       }
//       // Optional: sleep to control the frame rate
//       std::this_thread::sleep_for(std::chrono::milliseconds(30));
//     }
//   }
//
//   std::string stream_url;
//   size_t buffer_size;
//   std::thread capture_thread;
//   std::atomic<bool> stop_thread;
//   std::deque<cv::Mat> frame_buffer; // Circular buffer to store recent frames
//   std::mutex frame_mutex;
//   std::condition_variable frame_cv;
// };
