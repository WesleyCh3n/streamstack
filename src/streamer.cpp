// #include "streamer.hpp"
//
// #include <atomic>
// #include <thread>
//
// using namespace std::chrono_literals;
//
// #include <opencv2/cudacodec.hpp>
// #include <opencv2/opencv.hpp>
//
// #include <iostream>
// using std::cout;
//
// class GPUStreamerThread::Impl {
//   std::thread stream_thread_;
//   std::thread main_thread_;
//   std::atomic<bool> running_{false};
//   DataPipe *pipe_{nullptr};
//
//   StreamerConfig config_;
//   std::array<cv::Mat, 2> frames_;
//   std::mutex frames_mutex_;
//   size_t frame_index_;
//
// public:
//   Impl(StreamerConfig config, DataPipe *pipe) : config_(config), pipe_(pipe)
//   {
//     running_.store(true);
//     main_thread_ = std::thread(&Impl::main_loop, this);
//     stream_thread_ = std::thread(&Impl::frame_loop, this);
//   }
//   ~Impl() { stop(); };
//
//   void stop();
//   bool is_running();
//
// private:
//   void frame_loop();
//   void main_loop();
// };
//
// bool GPUStreamerThread::Impl::is_running() { return running_; }
// void GPUStreamerThread::Impl::stop() {
//   running_.store(false);
//   if (main_thread_.joinable())
//     main_thread_.join();
//
//   if (stream_thread_.joinable())
//     stream_thread_.join();
// }
//
// auto open_cam(const std::string &uri) -> cv::Ptr<cv::cudacodec::VideoReader>
// {
//   cv::cudacodec::VideoReaderInitParams params;
//   auto cam = cv::cudacodec::createVideoReader(uri, {}, params);
//   return cam;
// }
//
// void GPUStreamerThread::Impl::frame_loop() {
//   // open device
//   while (running_) {
//     // if device not open
//     //    open device
//     //
//
//     // if grab frame success
//     {
//       std::lock_guard<std::mutex> lock(frames_mutex_);
//       frames_[frame_index_] =
//           cv::Mat::eye(2 + frame_index_, 2 + frame_index_, CV_8U);
//       frame_index_ = !frame_index_;
//     }
//     std::this_thread::sleep_for(200ms); // FPS control
//   }
// }
//
// void GPUStreamerThread::Impl::main_loop() {
//   cv::Mat frame;
//   while (running_) {
//     cout << "capture frame\n";
//     {
//       std::lock_guard<std::mutex> lock(frames_mutex_);
//       cv::swap(frame, frames_[!frame_index_]);
//     }
//
//     DetectionTask task;
//     task.mat = frame;
//     auto future = task.get_future();
//     pipe_->detection_queue.try_enqueue(std::move(task));
//
//     std::future_status status;
//     if ((status = future.wait_for(0.1s)) == std::future_status::ready) {
//       auto mats = future.get();
//       cout << "deq result vec: " << mats.data() << '\n';
//       cout << "deq result vec[0]: " << static_cast<void *>(mats[0].data)
//            << '\n';
//     }
//     // TODO: full time loop
//     std::this_thread::sleep_for(500ms); // FPS control
//   }
// }
//
// GPUStreamerThread::GPUStreamerThread(StreamerConfig config, DataPipe *pipe)
//     : impl_(std::make_unique<Impl>(config, pipe)) {}
// GPUStreamerThread::~GPUStreamerThread() noexcept {}
// GPUStreamerThread::GPUStreamerThread(GPUStreamerThread &&other) noexcept
//     : impl_(std::move(other.impl_)) {}
// void GPUStreamerThread::stop() { impl_->stop(); }
// bool GPUStreamerThread::is_running() { return impl_->is_running(); }
