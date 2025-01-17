CV_VERSION ?= 4.10.0
BUILD_TYPE ?= Release

all:

test: build/test/
	ctest --output-on-failure --test-dir ./build/test/ --verbose

opencv:
	make -C build -f opencv.mk \
		VERSION=$(CV_VERSION) \
		BUILD_TYPE=$(BUILD_TYPE)
