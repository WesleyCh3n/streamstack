#pragma once

#include "concurrentqueue.h"

template <typename T> using Datapipe = moodycamel::ConcurrentQueue<T>;

// struct DataPipe : public noncopyable {
//   DetectionQueue detection_queue;
// };
