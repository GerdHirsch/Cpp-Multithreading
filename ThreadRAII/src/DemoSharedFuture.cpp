/*
 * DemoSharedFuture.cpp
 *
 *  Created on: 08.04.2018
 *      Author: Gerd
 */

#include "ThreadRAII.h"

#include <iostream>
#include <thread>
#include <future>
using namespace std;

//---------------------------------------------------------------------
// see also project DemoAsync DemoSharedFuture.cpp
int myIntTask(){
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	std::this_thread::sleep_for(10s);
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

	auto future = std::async(std::launch::async, myIntTask);
	auto sf = future.share();

	ThreadRAII<> tr0( ( std::thread( useSharedFuture<int>, sf ) ) );
	ThreadRAII<> tr1( ( std::thread( useSharedFuture<int>, sf ) ) );
	ThreadRAII<> tr2( ( std::thread( useSharedFuture<int>, sf ) ) );

}


