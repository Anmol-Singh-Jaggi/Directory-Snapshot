# Change these according to the Boost installation path in your system
BOOST_HEADER_PATH="${HOME}/.boost_home/include"
BOOST_OBJECT_PATH="${HOME}/.boost_home/lib"

g++ -O3 -flto -std=c++11 -fexceptions -isystem "${BOOST_HEADER_PATH}" -c directory_snapshot.cpp -o directory_snapshot.o
g++ -O3 -flto -o directory_snapshot directory_snapshot.o "${BOOST_OBJECT_PATH}/libboost_filesystem.a" "${BOOST_OBJECT_PATH}/libboost_system.a"
