CV_VERSION ?= 4.10.0
BUILD_TYPE ?= Release

all:

test: build/test/
	ctest  -E CameraTest* --output-on-failure --test-dir ./build/test/ --verbose

coverage: build/
	ctest --verbose -T Coverage --test-dir ./build/
	lcov -c -d ./build -o build/cov-complete.info
	lcov --remove build/cov-complete.info -o build/cov-filtered.info \
		'$(shell pwd)/build/_deps/*' \
		'/usr/include/*' \
		'11/*'
	genhtml build/cov-filtered.info -o build/html-report
	tar cf build/html-report.tar -C build html-report

opencv:
	make -C build -f $(shell pwd)/3rdparty/opencv4.mk \
		VERSION=$(CV_VERSION) \
		BUILD_TYPE=$(BUILD_TYPE)
