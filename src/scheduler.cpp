#include "scheduler.hpp"
#include <thread>

#include <iostream>
using std::cout;

class DetectionScheduler::Impl {
  std::thread t_;
  std::atomic<bool> running_{false};
  DetectionQueue *queue_;

public:
  Impl(DetectionQueue *queue) : queue_(queue) {
    t_ = std::thread(&Impl::loop, this);
  }
  ~Impl() { stop(); }

private:
  void loop() {
    running_.store(true);
    while (running_) {
      DetectionTask tasks[20];
      int count = queue_->try_dequeue_bulk(tasks, 20);
      if (count > 0) {
        cout << "count: " << count << '\n';
        for (int i = 0; i < count; i++) {
          cout << "queue mat: " << static_cast<void *>(tasks[i].mat.data)
               << '\n';
          std::vector<cv::Mat> mats = {cv::Mat::ones(4, 4, CV_8U)};
          tasks[i].set_value(std::move(mats));
        }
      } else {
        // wait some time
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
      }
    }
  }
  void stop() {
    running_.store(false);
    if (t_.joinable()) {
      t_.join();
    }
  }
};

DetectionScheduler::DetectionScheduler(DetectionQueue *queue)
    : impl_(std::make_unique<Impl>(queue)) {}
DetectionScheduler::~DetectionScheduler() {}
