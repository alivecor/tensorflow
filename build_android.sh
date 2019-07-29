#!/bin/bash

for ARCH in "x86" "x86_64" "armeabi-v7a" "arm64-v8a"
do
	bazel build -c opt //tensorflow/contrib/android:libtensorflow_inference.so \
		--crosstool_top=//external:android/crosstool \
		--host_crosstool_top=@bazel_tools//tools/cpp:toolchain \
		--cpu=$ARCH
done
