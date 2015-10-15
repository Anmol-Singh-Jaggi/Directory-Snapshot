# Change these according to the Boost installation path in your system
BOOST_HEADER_PATH=~/boost
BOOST_OBJECT_PATH=~/boost/stage/lib

g++ -std=c++11 -fexceptions -isystem $BOOST_HEADER_PATH -c directory_snapshot.cpp -o directory_snapshot.o
g++ -o directory_snapshot directory_snapshot.o $BOOST_OBJECT_PATH/libboost_filesystem.a $BOOST_OBJECT_PATH/libboost_system.a

