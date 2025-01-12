#include <iostream>
#include <thread>

#include "concurrentqueue.h"
#include "datapipe.hpp"
#include "scheduler.hpp"
#include "streamer.hpp"

// #include <opencv2/opencv.hpp>

using moodycamel::ConcurrentQueue;
using std::cout;

class MyThread {
  std::unique_ptr<std::thread> thread_;

public:
  void start() {
    thread_ = std::make_unique<std::thread>(std::thread(&MyThread::main, this));
  }
  virtual void main() = 0;
  void wait() {
    if (thread_ and thread_->joinable()) {
      thread_->join();
    }
  }
};

#define GPU_TASK_PER_SEC 10
class TaskScheduler : public MyThread {
  ConcurrentQueue<int> &q_;
  std::atomic<bool> running_{false};

public:
  TaskScheduler(ConcurrentQueue<int> &q) : q_(q) {}
  void main() override {
    running_.store(true);
    while (running_) {
      int req[GPU_TASK_PER_SEC];
      int count = q_.try_dequeue_bulk(req, GPU_TASK_PER_SEC);
      if (count > 0) {
        // process request with GPU manager
        for (int i = 0; i < count; i++) {
          std::cout << req[i] << ' ';
        }
        std::cout << '\n';
        std::this_thread::sleep_for(
            std::chrono::milliseconds(count * (1 / GPU_TASK_PER_SEC)));
      } else {
        // wait some time
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
      }
    }
  }
  void stop() { running_.store(false); }
};

class Client {
  int id_ = 0;
  ConcurrentQueue<int> &q_;

public:
  Client(ConcurrentQueue<int> &q, int id) : q_(q), id_(id) {}
  void run() {
    for (int i = 0; i < 20; i++) {
      q_.try_enqueue(id_ + i);
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
  }
};

int main(int argc, char *argv[]) {
  DataPipe pipe;
  std::vector<GPUStreamerThread> streamers;
  for (int i = 0; i < 1; i++) {
    streamers.emplace_back(GPUStreamerThread(StreamerConfig{}, &pipe));
  }
  // for (int i = 0; i < 1; i++) {
  //   streamers.push_back(std::move(GPUStreamerThread(StreamerConfig{},
  //   &pipe)));
  // }
  DetectionScheduler dscheduler(&pipe.detection_queue);
  std::this_thread::sleep_for(std::chrono::seconds(3));
  // detection thread (detection queue)
  //  --> start
  //  --> stop
  // classification thread (classification queue)
  //  --> start
  //  --> stop
  return 0;
}
