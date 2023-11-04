cmake_minimum_required(VERSION 3.26)
project(mbc)

set(CMAKE_CXX_STANDARD 17)

add_executable(mbc
        mbc/main.cpp
		mbc/mbc.hpp
        mbc/Graph.cpp
        mbc/Graph.h)

add_executable(
	graphTest
	test/testGraph.cpp
	mbc/Graph.h
	mbc/Graph.cpp)
