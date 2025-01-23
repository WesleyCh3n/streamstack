#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "datapipe.hpp"
#include "task.hpp"

TEST(DataPipeTest, EnqueueSingle) {
  using Item = Task<std::vector<int>, void *>;
  Item item = Item();
  item.set_request(std::vector<int>{1, 2, 3, 4});
  Datapipe<Item> pipe;
  ASSERT_TRUE(pipe.try_enqueue(std::move(item)));
}

TEST(DataPipeTest, EnqueueData) {
  using Item = Task<int, void *>;
  Datapipe<std::unique_ptr<Item>> pipe;
  auto item = std::make_unique<Item>();
  ASSERT_TRUE(pipe.try_enqueue(std::move(item)));
}

TEST(DataPipeTest, DequeueData) {
  using Item = Task<int, void *>;
  Datapipe<std::unique_ptr<Item>> pipe;
  auto item = std::make_unique<Item>();
  item->set_request(10);
  auto before = item.get();
  ASSERT_TRUE(pipe.try_enqueue(std::move(item)));

  auto got = std::make_unique<Item>();
  ASSERT_TRUE(pipe.try_dequeue(got));
  ASSERT_EQ(got->get_request(), 10);
}

TEST(DataPipeTest, DataIsSame) {
  using Item = Task<int, void *>;
  Datapipe<std::unique_ptr<Item>> pipe;
  auto item = std::make_unique<Item>();
  item->set_request(10);
  auto before = item.get();
  ASSERT_TRUE(pipe.try_enqueue(std::move(item)));

  auto got = std::make_unique<Item>();
  ASSERT_TRUE(pipe.try_dequeue(got));
  ASSERT_EQ(got->get_request(), 10);
  ASSERT_EQ(before, got.get());
}
