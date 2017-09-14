#!/bin/bash -x
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
# Builds the TensorFlow core library with x86 architecture for MacOSX


function less_than_required_version() {
  echo $1 | (IFS=. read major minor micro
    if [ $major -ne $2 ]; then
      [ $major -lt $2 ]
    elif [ $minor -ne $3 ]; then
      [ $minor -lt $3 ]
    else
      [ ${micro:-0} -lt $4 ]
    fi
  )
}

ACTUAL_XCODE_VERSION=`xcodebuild -version | head -n 1 | sed 's/Xcode //'`
REQUIRED_XCODE_VERSION=7.3.0
if less_than_required_version $ACTUAL_XCODE_VERSION 7 3 0
then
    echo "error: Xcode ${REQUIRED_XCODE_VERSION} or later is required."
    exit 1
fi

GENDIR=tensorflow/contrib/makefile/gen/
LIBDIR=${GENDIR}lib
LIB_PREFIX=libtensorflow-core

CXXFLAGS="--std=c++11 -DIS_SLIM_BUILD -fno-exceptions -DNDEBUG -DNOTFDBG -O3 -Wno-unknown-warning-option -Wno-invalid-partial-specialization -Ltensorflow/contrib/makefile/gen/protobuf-host/lib -DSELECTIVE_REGISTRATION -DSUPPORT_SELECTIVE_REGISTRATION"
CXX="clang++"

make -f tensorflow/contrib/makefile/Makefile \
TARGET=OSX LIB_NAME=${LIB_PREFIX}.a CXX="${CXX}" CXXFLAGS="${CXXFLAGS}" \
OPTFLAGS="$1" $2 $3 lib_only


if [ $? -ne 0 ]
then
  echo "x86_64 compilation failed."
  exit 1
fi

