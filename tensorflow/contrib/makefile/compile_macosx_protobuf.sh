#!/bin/bash -x -e
# Copyright 2015 The TensorFlow Authors. All Rights Reserved.
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
# Builds protobuf 3 for mac OSX.

SCRIPT_DIR=$(dirname $0)
source "${SCRIPT_DIR}/build_helper.subr"

cd tensorflow/contrib/makefile

HOST_GENDIR="$(pwd)/gen/protobuf-host"
mkdir -p "${HOST_GENDIR}"
if [[ ! -f "./downloads/protobuf/autogen.sh" ]]; then
    echo "You need to download dependencies before running this script." 1>&2
    echo "tensorflow/contrib/makefile/download_dependencies.sh" 1>&2
    exit 1
fi

if [ "$#" -gt 1 ]; then
    JOBS_COUNT=$1
else
    JOBS_COUNT=4
fi

GENDIR=`pwd`/gen/protobuf/
LIBDIR=${GENDIR}lib
mkdir -p ${LIBDIR}

OSX_VERSION=darwin14.0.0

MACOSX_PLATFORM=`xcrun --sdk macosx --show-sdk-platform-path`
MACOSX_SYSROOT=`xcrun --sdk macosx --show-sdk-path`
MACOSX_SDK_VERSION=`xcrun --sdk macosx --show-sdk-version`
MIN_SDK_VERSION=10.10

CC=clang
CXX=clang++
CFLAGS="-DNDEBUG -O3 -pipe -fPIC -fno-exceptions"
CXXFLAGS="${CFLAGS} -std=c++11 -stdlib=libc++ -arch x86_64"
LDFLAGS="-stdlib=libc++ -arch x86_64"
LIBS="-lc++ -lc++abi"

cd downloads/protobuf
PROTOC_PATH="${HOST_GENDIR}/bin/protoc"
if [[ ! -f "${PROTOC_PATH}" || ${clean} == true ]]; then
  # Try building compatible protoc first on host
  echo "protoc not found at ${PROTOC_PATH}. Build it first."
  make_host_protoc "${HOST_GENDIR}"
else
  echo "protoc found. Skip building host tools."
fi

./autogen.sh
if [ $? -ne 0 ]
then
  echo "./autogen.sh command failed."
  exit 1
fi


./configure \
--disable-shared \
CFLAGS="${CFLAGS}" \
CXXFLAGS="${CXXFLAGS}" \
LDFLAGS="${LDFLAGS}"

make clean

make -j ${JOBS_COUNT}

make install
