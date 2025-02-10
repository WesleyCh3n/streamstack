#include "utils.hpp"

#include "datapipe.hpp"
#include "task.hpp"

TEST(DataPipeTest, EnqueueTask) {
  Datapipe<Task<Unique, Empty>> pipe;
  ASSERT_TRUE(pipe.try_enqueue(Task<Unique, Empty>(std::make_unique<int>(1))));
}

TEST(DataPipeTest, EnqueueMat) {
  Datapipe<Task<cv::Mat, Empty>> pipe;
  ASSERT_TRUE(
      pipe.try_enqueue(Task<cv::Mat, Empty>(cv::Mat::ones(5, 5, CV_8UC3))));
}

TEST(DataPipeTest, DequeueTaskOfVecIsSame) {
  using TaskData = Task<Vec, Empty>;

  Vec enq_vec{1, 2, 3, 4}, deq_vec{};
  auto before{enq_vec.data()};
  TaskData enq_task{enq_vec}, deq_task{Vec{}};
  ASSERT_TRUE(enq_vec.empty());

  Datapipe<TaskData> pipe;
  ASSERT_TRUE(pipe.try_enqueue(std::move(enq_task)));
  ASSERT_TRUE(pipe.try_dequeue(deq_task));

  deq_task.take(deq_vec);
  ASSERT_THAT(deq_vec, testing::ElementsAre(1, 2, 3, 4));
  ASSERT_EQ(before, deq_vec.data());
}

TEST(DataPipeTest, DequeueTaskOfMatIsSame) {
  using TaskData = Task<cv::Mat, Empty>;

  const cv::Mat mat{cv::Mat::ones(5, 5, CV_8UC3)};
  cv::Mat enq_mat{mat}, deq_mat{};
  auto before{enq_mat.data};
  TaskData enq_task{enq_mat}, deq_task{cv::Mat{}};
  ASSERT_TRUE(enq_mat.empty());

  Datapipe<TaskData> pipe;
  ASSERT_TRUE(pipe.try_enqueue(std::move(enq_task)));
  ASSERT_TRUE(pipe.try_dequeue(deq_task));

  deq_task.take(deq_mat);
  ASSERT_TRUE(mat_eq(mat, deq_mat));
  ASSERT_EQ(before, deq_mat.data);
}
