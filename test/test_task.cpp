#include "utils.hpp"

#include "task.hpp"

class TaskTest : public testing::Test {};

TEST_F(TaskTest, CtorData) { Task<int, Empty> task(1); }

TEST_F(TaskTest, CtorDataInt) {
  int input{1}, output{0};
  Task<int, Empty> task(input);
  task.take(output);
  ASSERT_EQ(output, 1);
}

TEST_F(TaskTest, CtorDataNullptr) {
  Unique input{nullptr}, output{nullptr};
  Task<Unique, Empty> task(nullptr);
  task.take(output);
  ASSERT_EQ(output, nullptr);
}

TEST_F(TaskTest, TakeDataValidate) {
  Unique input{std::make_unique<int>(10)}, output(nullptr);
  Task<Unique, Empty> task(input);
  ASSERT_EQ(input, nullptr);
  task.take(output);
  ASSERT_NE(output, nullptr);
  ASSERT_EQ(*output, 10);
}

TEST_F(TaskTest, GetRequestUniquePtrOwnership) {
  Unique before{std::make_unique<int>(10)}, after{nullptr};
  auto before_addr = before.get(); // address of the original pointer
  Task<Unique, Empty> task(before);
  ASSERT_EQ(before, nullptr); // After move, 'before' should be null

  task.take(after); // 'task' should now own the unique_ptr
  ASSERT_NE(after, nullptr);
  ASSERT_EQ(after.get(), before_addr);
}

TEST_F(TaskTest, GetRequestVectorOwnership) {
  Vec before{0, 1, 2}, after;
  auto before_addr = before.data(); // address of the original pointer
  Task<Vec, Empty> task(before);
  ASSERT_TRUE(before.empty());       // After move, 'before' should be empty
  ASSERT_EQ(before.data(), nullptr); // After move, 'before.data' should be null

  task.take(after); // 'task' should now own the unique_ptr
  ASSERT_EQ(after.data(), before_addr);
  ASSERT_THAT(after, testing::ElementsAre(0, 1, 2));
}

TEST_F(TaskTest, SetResponseOnce) {
  Task<Empty, Empty> task(nullptr);
  task.set_response(nullptr);
}

TEST_F(TaskTest, SetResponseMultiple) {
  Task<Empty, Empty> task(nullptr);
  task.set_response(nullptr);
  ASSERT_THROW(
      {
        try {
          task.set_response(nullptr);
        } catch (std::exception const &e) {
          ASSERT_STREQ(e.what(),
                       "std::future_error: Promise already satisfied");
          throw;
        }
      },
      std::future_error);
}

TEST_F(TaskTest, SetResponseValidateValue) {
  Task<Empty, Unique> task(nullptr);
  task.set_response(std::make_unique<int>(2));
  auto f = task.get_response();
  ASSERT_EQ(*f.get().get(), 2);
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
