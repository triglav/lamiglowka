#!/bin/sh
set -ex
wget https://github.com/google/glog/archive/v0.3.5.tar.gz -O glog-0.3.5.tar.gz
tar -xzvf glog-0.3.5.tar.gz
cd glog-0.3.5 && mkdir build && cd build
cmake ..
cmake --build .

