#include <thread>

// #include "concurrentqueue.h"
#include "task.hpp"

#include <gtest/gtest.h>
#include <opencv2/opencv.hpp>

using std::thread;
using namespace std::chrono_literals;

TEST(TaskCollection, DetectionTaskPromiseAddrress) {
  DetectionTask detection_task;
  auto future = detection_task.get_future();
  cv::Mat *input_addr = nullptr, *output_addr = nullptr;
  thread t(
      [&input_addr](DetectionTask &&task) {
        Data input = {cv::Mat()};
        input_addr = input.data();
        task.set_value(std::move(input));
      },
      std::move(detection_task));
  auto output = future.get();
  output_addr = output.data();
  ASSERT_EQ(input_addr, output_addr);
  t.join();
}

// int main(int argc, char *argv[]) {
//   using pro = MyObject;
//   moodycamel::ConcurrentQueue<pro> queue;
//
//   std::thread t([&]() {
//     pro objs[5];
//     while (true) {
//       int count = queue.try_dequeue_bulk(objs, 5);
//       if (count == 0)
//         continue;
//       for (int i = 0; i < count; i++) {
//         Data v = {cv::Mat::ones(2, 2, CV_8U)};
//         cout << "in: " << v.data() << '\n';
//         cout << "in data[0]: " << static_cast<void *>(v[0].data) << '\n';
//         objs[i].set_value(std::move(v));
//       }
//       break;
//     }
//   });
//
//   pro obj;
//   auto future = obj.get_future();
//   queue.try_enqueue(std::move(obj));
//
//   cout << "start wait\n";
//   std::cout << "valid: " << future.valid() << '\n';
//   Data v = {};
//   v = future.get();
//   cout << "out: " << v.data() << '\n';
//   cout << "out data[0]: " << static_cast<void *>(v[0].data) << '\n';
//   t.join();
// }
