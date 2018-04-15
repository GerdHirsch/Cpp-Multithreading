/*
 * Tasks.cpp
 *
 *  Created on: 15.04.2018
 *      Author: Gerd
 */

#include "Tasks.h"

#include <chrono>
#include <future>
#include <thread>
#include <vector>
#include <iostream>
using namespace std;

void task1(std::shared_future<bool> future){

}

template<bool coutLock = true>
void task0(std::mutex& coutMutex, std::chrono::milliseconds delay, std::shared_future<void> future ){
#define CATCHEXCEPTION

	using Lock = std::conditional_t<coutLock, std::unique_lock<std::mutex>, Demo::NoLock<std::mutex>>;

	{
		Lock lk(coutMutex);
		cout << endl<< __PRETTY_FUNCTION__  << endl;
		cout  << "thread id: " << this_thread::get_id() << endl;
	}
	this_thread::sleep_for(delay);
#ifdef CATCHEXCEPTION // demo system behavior if you don´t catch it
	try{
#endif
		{
			Lock lk(coutMutex);
			cout << endl << "before future.get() " << "thread id: " << this_thread::get_id() << endl;
		}
//			future.wait(); // #1
//		bool joined = future.get(); // #1

		Lock lk(coutMutex);
//		cout << "after future.get() joined: " << joined;
//			cout << "after future.wait()";
		cout << " thread id: " << this_thread::get_id()
			<< endl;
#ifdef CATCHEXCEPTION
	}catch(std::future_error& e){
		Lock lk(coutMutex);
		cout << "lambda catch(std::future_error&): " << e.what() << " thread id: " << this_thread::get_id()<< endl;
	}catch(...){
		Lock lk(coutMutex);
		cout << "lambda catch(...): " << " thread id: " << this_thread::get_id()<< endl;
	}
#endif
}

