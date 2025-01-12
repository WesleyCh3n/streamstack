#pragma once

#include "task.hpp"

#include "concurrentqueue.h"
#include "noncopyable.hpp"

using DetectionQueue = moodycamel::ConcurrentQueue<DetectionTask>;

struct DataPipe : public noncopyable {
  DetectionQueue detection_queue;
};
