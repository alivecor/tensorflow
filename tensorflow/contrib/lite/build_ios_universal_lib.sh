#!/bin/bash -x
# Copyright 2017 The TensorFlow Authors. All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# ==============================================================================

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR/../../.."

make -f tensorflow/contrib/lite/Makefile TARGET=IOS IOS_ARCH=x86_64 -j 8 \
$SCRIPT_DIR/gen/lib/ios_x86_64/libtensorflow-lite.a
make -f tensorflow/contrib/lite/Makefile TARGET=IOS IOS_ARCH=i386 -j 8 \
$SCRIPT_DIR/gen/lib/ios_i386/libtensorflow-lite.a
make -f tensorflow/contrib/lite/Makefile TARGET=IOS IOS_ARCH=armv7k -j 8 \
$SCRIPT_DIR/gen/lib/ios_armv7k/libtensorflow-lite.a
make -f tensorflow/contrib/lite/Makefile TARGET=IOS IOS_ARCH=arm64_32 -j 8 \
$SCRIPT_DIR/gen/lib/ios_arm64_32/libtensorflow-lite.a

mkdir -p tensorflow/contrib/lite/gen/lib/ios/ 

lipo \
tensorflow/contrib/lite/gen/lib/ios_x86_64/libtensorflow-lite.a \
tensorflow/contrib/lite/gen/lib/ios_i386/libtensorflow-lite.a \
tensorflow/contrib/lite/gen/lib/ios_armv7k/libtensorflow-lite.a \
tensorflow/contrib/lite/gen/lib/ios_arm64_32/libtensorflow-lite.a \
-create \
-output tensorflow/contrib/lite/gen/lib/ios/libtensorflow-lite.a
