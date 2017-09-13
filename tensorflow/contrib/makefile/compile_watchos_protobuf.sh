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
# Builds protobuf 3 for iOS.

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

export MACOSX_DEPLOYMENT_TARGET="10.10"
export WATCHOS_DEPLOYMENT_TARGET="4.0"

GENDIR=`pwd`/gen/protobuf_watchos/
LIBDIR=${GENDIR}lib
mkdir -p ${LIBDIR}

OSX_VERSION=darwin14.0.0

WATCHOS_PLATFORM=`xcrun --sdk watchos --show-sdk-platform-path`
WATCHOS_SYSROOT=`xcrun --sdk watchos --show-sdk-path`
WATCHSIMULATOR_PLATFORM=`xcrun --sdk watchsimulator --show-sdk-platform-path`
WATCHSIMULATOR_SYSROOT=`xcrun --sdk watchsimulator --show-sdk-path`
IOS_SDK_VERSION=`xcrun --sdk watchos --show-sdk-version`
MIN_SDK_VERSION=4.0

CFLAGS="-DNDEBUG -Os -pipe -fPIC -fno-exceptions -fembed-bitcode"
CXXFLAGS="${CFLAGS}"
CXX="clang++ -std=c++11"
LDFLAGS="-stdlib=libc++"
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

make distclean
./configure \
--host=i386-apple-${OSX_VERSION} \
--disable-shared \
--enable-cross-compile \
--with-protoc="${PROTOC_PATH}" \
--prefix=${LIBDIR}/watchossim_386 \
--exec-prefix=${LIBDIR}/watchossim_386 \
"CFLAGS=${CFLAGS} \
-mwatchos-simulator-version-min=${MIN_SDK_VERSION} \
-arch i386 \
-isysroot ${WATCHSIMULATOR_SYSROOT}" \
"CXX=${CXX}" \
"CXXFLAGS=${CXXFLAGS} \
-mwatchos-simulator-version-min=${MIN_SDK_VERSION} \
-arch i386 \
-isysroot \
${WATCHSIMULATOR_SYSROOT}" \
LDFLAGS="-arch i386 \
-mwatchos-simulator-version-min=${MIN_SDK_VERSION} \
${LDFLAGS} \
-L${WATCHSIMULATOR_SYSROOT}/usr/lib/ \
-L${WATCHSIMULATOR_SYSROOT}/usr/lib/system" \
"LIBS=${LIBS}"
make -j ${JOBS_COUNT}
make install

make distclean
./configure \
--host=x86_64-apple-${OSX_VERSION} \
--disable-shared \
--enable-cross-compile \
--with-protoc="${PROTOC_PATH}" \
--prefix=${LIBDIR}/watchossim_x86_64 \
--exec-prefix=${LIBDIR}/watchossim_x86_64 \
"CFLAGS=${CFLAGS} \
-mwatchos-simulator-version-min=${MIN_SDK_VERSION} \
-arch x86_64 \
-isysroot ${WATCHSIMULATOR_SYSROOT}" \
"CXX=${CXX}" \
"CXXFLAGS=${CXXFLAGS} \
-mwatchos-simulator-version-min=${MIN_SDK_VERSION} \
-arch x86_64 \
-isysroot \
${WATCHSIMULATOR_SYSROOT}" \
LDFLAGS="-arch x86_64 \
-mwatchos-simulator-version-min=${MIN_SDK_VERSION} \
${LDFLAGS} \
-L${WATCHSIMULATOR_SYSROOT}/usr/lib/ \
-L${WATCHSIMULATOR_SYSROOT}/usr/lib/system" \
"LIBS=${LIBS}"
make -j ${JOBS_COUNT}
make install

make distclean
./configure \
--host=armv7k-apple-${OSX_VERSION} \
--with-protoc="${PROTOC_PATH}" \
--disable-shared \
--prefix=${LIBDIR}/watchos_armv7k \
--exec-prefix=${LIBDIR}/watchos_armv7k \
"CFLAGS=${CFLAGS} \
-mwatchos-version-min=${MIN_SDK_VERSION} \
-arch armv7k \
-isysroot ${WATCHOS_SYSROOT}" \
"CXX=${CXX}" \
"CXXFLAGS=${CXXFLAGS} \
-mwatchos-version-min=${MIN_SDK_VERSION} \
-arch armv7k \
-isysroot ${WATCHOS_SYSROOT}" \
LDFLAGS="-arch armv7k \
-mwatchos-version-min=${MIN_SDK_VERSION} \
${LDFLAGS}" \
"LIBS=${LIBS}"
make -j ${JOBS_COUNT}
make install

lipo \
${LIBDIR}/watchossim_386/lib/libprotobuf.a \
${LIBDIR}/watchossim_x86_64/lib/libprotobuf.a \
${LIBDIR}/watchos_armv7k/lib/libprotobuf.a \
-create \
-output ${LIBDIR}/libprotobuf.a

lipo \
${LIBDIR}/watchossim_386/lib/libprotobuf-lite.a \
${LIBDIR}/watchossim_x86_64/lib/libprotobuf-lite.a \
${LIBDIR}/watchos_armv7k/lib/libprotobuf-lite.a \
-create \
-output ${LIBDIR}/libprotobuf-lite.a
