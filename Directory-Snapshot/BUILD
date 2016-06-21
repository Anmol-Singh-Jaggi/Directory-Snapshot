#!/usr/bin/env bash

# Change these according to the Boost installation path in your system
BOOST_HEADER_PATH="${HOME}/.boost_home/include"
BOOST_OBJECT_PATH="${HOME}/.boost_home/lib"

TARGET="directory-snapshot"

g++ -O3 -flto -std=c++11 -fexceptions -isystem "${BOOST_HEADER_PATH}" -c "src/${TARGET}.cpp" -o "bin/${TARGET}.o"
g++ -O3 -flto -o "bin/${TARGET}" "bin/${TARGET}.o" "${BOOST_OBJECT_PATH}/libboost_filesystem.a" "${BOOST_OBJECT_PATH}/libboost_system.a"
