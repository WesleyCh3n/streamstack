VERSION ?= 4.10.0

BUILD_TYPE ?= Release
INSTALL_PREFIX := /opt/opencv$(VERSION)
BUILD_DIR := opencv_build

OPENCV_SRC_URL = https://github.com/opencv/opencv/archive/refs/tags/$(VERSION).tar.gz
OPENCV_TAR = opencv.tar.gz
OPENCV_SRC_DIR = opencv-src

OPENCV_CONTRIB_SRC_URL = https://github.com/opencv/opencv_contrib/archive/refs/tags/$(VERSION).tar.gz
OPENCV_CONTRIB_TARa = opencv_contrib.tar.gz
OPENCV_CONTRIB_SRC_DIR = opencv_contrib-src/modules

all:
	@curl -L $(OPENCV_SRC_URL) -o $(OPENCV_TAR)
	@curl -L  $(OPENCV_CONTRIB_SRC_URL) -o $(OPENCV_CONTRIB_TAR)
	@mkdir -p $(OPENCV_SRC_DIR) && mkdir -p $(OPENCV_CONTRIB_SRC_DIR)
	@tar xf $(OPENCV_TAR) --strip-components=1 -C $(OPENCV_SRC_DIR)
	@tar xf $(OPENCV_CONTRIB_TAR) --strip-components=1 -C $(OPENCV_SRC_DIR)
	@cmake \
		-DCMAKE_BUILD_TYPE=$(BUILD_TYPE) \
		-DCMAKE_INSTALL_PREFIX=$(INSTALL_PREFIX) \
		-DWITH_CUDA=ON -DCUDA_TOOLKIT_ROOT_DIR:PATH=/usr/local/cuda-12.3 \
		-DWITH_CUDNN=ON -DWITH_CUBLAS=ON -DWITH_CUFFT=ON \
		-DWITH_NVCUVENC=ON -DWITH_NVCUVID=ON \
		-DOPENCV_DNN_CUDA=ON \
		-DOPENCV_ENABLE_NONFREE=ON \
		-DWITH_JPEG=ON -DBUILD_JPEG=ON \
		-DWITH_FFMPEG=ON -DVIDEOIO_PLUGIN_LIST=ffmpeg \
		-DOPENCV_EXTRA_MODULES_PATH=$(OPENCV_CONTRIB_SRC_DIR) \
		-DBUILD_LIST=core,dnn,videoio,cudev \
		-B $(BUILD_DIR) \
		$(OPENCV_SRC_DIR)
	cmake --build $(BUILD_DIR) -- -j$(shell expr $(nproc --all) / 2)

help:
	cmake \
		-LAH \
		-DWITH_CUDA=ON \
		-DOPENCV_EXTRA_MODULES_PATH=$(OPENCV_CONTRIB_SRC_DIR) \
		-B $(BUILD_DIR) \
		$(OPENCV_SRC_DIR)
