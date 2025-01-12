#pragma once

#include "datapipe.hpp"

class DetectionScheduler {
  class Impl;
  std::unique_ptr<Impl> impl_;

public:
  DetectionScheduler(DetectionQueue *);
  ~DetectionScheduler();
};

// class ClassficationHandler {
//   class Impl;
//   std::unique_ptr<Impl> impl_;
//
// public:
//   ClassficationHandler(ClassficationHandler *);
// };
