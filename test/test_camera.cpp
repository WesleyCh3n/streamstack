#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <opencv2/core.hpp>
#include <opencv2/cudacodec.hpp>
#include <opencv2/imgcodecs.hpp>
#include <thread>

#include <camera.hpp>
#include <cvutil.hpp>

using namespace std::chrono_literals;

const char *VALID_URL = "rtsp://admin:12345@192.168.11.126:554/stream1";
const char *INVALID_URL = "rtsp://admin:12345@192.168.11.126:554/stream";

class CameraTest : public testing::Test {
protected:
  std::unique_ptr<Camera> mycam;

  void SetUp() override { mycam = std::make_unique<Camera>(VALID_URL); }
  void TearDown() override { mycam.reset(); }
};

TEST_F(CameraTest, RetrieveGpuMat) {
  cv::cuda::GpuMat frame;
  mycam->retrieve(frame);
  ASSERT_FALSE(frame.empty());
}

TEST_F(CameraTest, RetrieveCpuMat) {
  cv::Mat frame;
  mycam->retrieve(frame);
  ASSERT_FALSE(frame.empty());
}

TEST_F(CameraTest, RetrieveMatType) {
  cv::cuda::GpuMat frame;
  mycam->retrieve(frame);
  ASSERT_EQ(frame.type(), CV_8UC3);
}

TEST_F(CameraTest, RetrieveOneMinute) {
  cv::cuda::GpuMat frame;

  const uint8_t fps = 30;
  const size_t second = 60;
  const size_t total_fcount = fps * second;

  for (int i = 0; i < total_fcount; i++) {
    mycam->retrieve(frame);
    ASSERT_FALSE(frame.empty());

    std::this_thread::sleep_for(std::chrono::milliseconds(1000 / fps));
  }
}

TEST_F(CameraTest, RetrieveFrameHighFPS) {
  const uint8_t fps = 30;
  const size_t second = 5;
  const size_t total_fcount = fps * second;

  cv::cuda::GpuMat frame;
  std::array<cv::Mat, total_fcount> cpu_frame;

  for (int i = 0; i < total_fcount; i++) {
    mycam->retrieve(frame);
    ASSERT_FALSE(frame.empty());

    cpu_frame[i] = cv::Mat(frame);
    ASSERT_FALSE(cpu_frame[i].empty());

    std::this_thread::sleep_for(std::chrono::milliseconds(1000 / fps));
  }

  util::create_mp4(cpu_frame, "retrieve-30fps.mp4", static_cast<double>(fps));
}

TEST_F(CameraTest, RetrieveFrameLowFPS) {
  const uint8_t fps = 5;
  const size_t second = 5;
  const size_t total_fcount = fps * second;

  cv::cuda::GpuMat frame;
  std::array<cv::Mat, total_fcount> cpu_frame;

  for (int i = 0; i < total_fcount; i++) {
    mycam->retrieve(frame);
    ASSERT_FALSE(frame.empty());

    cpu_frame[i] = cv::Mat(frame);
    ASSERT_FALSE(cpu_frame[i].empty());

    std::this_thread::sleep_for(std::chrono::milliseconds(1000 / fps));
  }

  util::create_mp4(cpu_frame, "retrieve-5fps.mp4", static_cast<double>(fps));
}

TEST_F(CameraTest, MultipleCamera) {
  const uint8_t fps = 5;
  const size_t second = 5;
  const size_t total_fcount = fps * second;

  const size_t thread_count = 5;
  std::array<std::thread, 5> threads;
  for (int i = 0; i < thread_count; i++) {
    threads[i] = std::thread([]() {
      Camera cam(VALID_URL);
      cv::cuda::GpuMat frame;
      std::array<cv::Mat, total_fcount> cpu_frame;

      for (int i = 0; i < total_fcount; i++) {
        cam.retrieve(frame);
        ASSERT_FALSE(frame.empty());

        cpu_frame[i] = cv::Mat(frame);
        ASSERT_FALSE(cpu_frame[i].empty());

        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / fps));
      }
    });
  }
  for (int i = 0; i < thread_count; i++) {
    threads[i].join();
  }
}

TEST_F(CameraTest, CameraFailedToOpened) {
  ASSERT_THROW(
      {
        try {
          Camera cam(INVALID_URL);
        } catch (cv::Exception &e) {
          EXPECT_STREQ(e.func.c_str(), "CuvidVideoSource");
          EXPECT_STREQ(e.err.c_str(), "CUDA_ERROR_FILE_NOT_FOUND [Code = 301]");
          throw;
        }
      },
      cv::Exception);
}
