cmake_minimum_required(VERSION 3.6)
project(DataStructure)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp BinTree.h HashTable.h LinkList.h Matrix.h SeqList.h SeqQueue.h SeqStack.h)
add_executable(DataStructure ${SOURCE_FILES})