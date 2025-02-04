#pragma once

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <opencv2/opencv.hpp>

using std::thread;
using namespace std::chrono_literals;

using Unique = std::unique_ptr<int>;
using Vec = std::vector<int>;
using Empty = void *;

