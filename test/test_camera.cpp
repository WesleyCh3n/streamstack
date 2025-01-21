#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <camera.hpp>
#include <opencv2/core.hpp>
#include <opencv2/cudacodec.hpp>
#include <opencv2/imgcodecs.hpp>
#include <thread>

using namespace std::chrono_literals;

const char *VALID_URL = "rtsp://admin:12345@192.168.11.126:554/stream1";
const char *INVALID_URL = "rtsp://admin:12345@192.168.11.126:554/stream";

class CameraTest : public testing::Test {
protected:
  std::unique_ptr<Camera> mycam;

  void SetUp() override { mycam = std::make_unique<Camera>(VALID_URL); }
  void TearDown() override { mycam.reset(); }
};

TEST_F(CameraTest, RetrieveFrameHighFPS) {
  const uint8_t fps = 30;
  cv::cuda::GpuMat frame;
  cv::Mat cpu_frame;
  for (int i = 0; i < fps; i++) {
    mycam->retrieve(frame);
    ASSERT_FALSE(frame.empty());
    cv::imwrite(std::string("frame-") + std::to_string(i) + ".jpg",
                cv::Mat(frame));
    std::this_thread::sleep_for(std::chrono::milliseconds(1000 / fps));
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
