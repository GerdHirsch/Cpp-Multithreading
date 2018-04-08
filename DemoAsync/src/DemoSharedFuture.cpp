/*
 * DemoSharedFuture.cpp
 *
 *  Created on: 08.04.2018
 *      Author: Gerd
 */

#include "reallyAsync.h"

#include <iostream>
#include <thread>
using namespace std;

//---------------------------------------------------------------------
// see also Project ThreadRAII DemoSharedFuture.cpp
int myIntTask(){
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	std::this_thread::sleep_for(5s);
	return 42;
}
template<typename T>
void useSharedFuture(std::shared_future<T> sf){
	std::cout << __PRETTY_FUNCTION__ << "Thread id: " << std::this_thread::get_id()  << std::endl;
	auto i = sf.get();
	std::cout << "Thread id: " << std::this_thread::get_id() << "i: " << i << std::endl;
}
void demoSharedFuture(){
	std::cout << __PRETTY_FUNCTION__ << "Thread id: " << std::this_thread::get_id() << std::endl;
	auto future = reallyAsync(myIntTask);
	auto sf = future.share();
	reallyAsync(useSharedFuture<int>, sf);
	reallyAsync(useSharedFuture<int>, sf);
	reallyAsync(useSharedFuture<int>, sf);
}

