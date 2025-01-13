#include <thread>

#include "datapipe.hpp"
#include "scheduler.hpp"
#include "streamer.hpp"

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
  std::this_thread::sleep_for(std::chrono::seconds(1));
  // detection thread (detection queue)
  //  --> start
  //  --> stop
  // classification thread (classification queue)
  //  --> start
  //  --> stop
  return 0;
}
