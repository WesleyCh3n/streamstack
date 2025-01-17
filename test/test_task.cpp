#include <thread>

// #include "concurrentqueue.h"
#include "task.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <opencv2/opencv.hpp>

using std::thread;
using namespace std::chrono_literals;

class TaskTest : public testing::Test {};

using std::unique_ptr;
using std::vector;

TEST_F(TaskTest, AssignUniquePtrOwnership) {
  using InType = unique_ptr<int>;
  InType before = std::make_unique<int>(10);
  int *before_addr = before.get(); // address of the original pointer
  Task<InType, void> task(std::move(before));
  ASSERT_EQ(before, nullptr);      // After move, 'before' should be null
  InType after = task.get_input(); // 'task' should now own the unique_ptr
  ASSERT_NE(after, nullptr);
  ASSERT_EQ(after.get(), before_addr);
}

TEST_F(TaskTest, AssignVectorOwnership) {
  using InType = vector<int>;
  InType before = {0, 1, 2};
  auto before_addr = before.data(); // address of the original pointer
  Task<InType, void> task(std::move(before));
  ASSERT_TRUE(before.empty());       // After move, 'before' should be empty
  ASSERT_EQ(before.data(), nullptr); // After move, 'before.data' should be null

  InType after = task.get_input(); // 'task' should now own the unique_ptr
  ASSERT_EQ(after.data(), before_addr);
  ASSERT_THAT(after, testing::ElementsAre(0, 1, 2));
}

TEST_F(TaskTest, AssignInputValidateValue) {
  auto before = std::make_unique<int>(10);
  Task<unique_ptr<int>, unique_ptr<int>> task(std::move(before));
  auto after = task.get_input();
  ASSERT_NE(after, nullptr);
  ASSERT_EQ(*after, 10);
}

TEST_F(TaskTest, AssignInputEmptyPtr) {
  auto empty_ptr = nullptr; // A nullptr
  Task<unique_ptr<int>, unique_ptr<int>> task(std::move(empty_ptr));
  ASSERT_EQ(task.get_input(), nullptr);
}

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
