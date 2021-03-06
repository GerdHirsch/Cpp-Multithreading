/*
 * DemoPromiseSharedFuture.cpp
 *
 *  Created on: 12.04.2018
 *      Author: Gerd
 */

#include "ThreadRAII.h"

#include <type_traits>
#include <future>
#include <thread>
#include <vector>
#include <iostream>
using namespace std;


namespace{
template<typename Mutex>
struct NoLock{
	NoLock(Mutex& m){}
};
}

void demoPromiseSharedFuture(){
	// different scenarios
// ugly works only for demo purposes
#define CATCHEXCEPTION
	const unsigned int numThreads = std::thread::hardware_concurrency();

	bool activateBefore{true};
	bool throwException{true};

	constexpr bool join{true};
	using Policy = std::conditional_t<join, JoinPolicy, DetachPolicy>;
	using TRAII = ThreadRAII<Policy>;

	constexpr bool coutLock = true;
	using Lock = std::conditional_t<coutLock, std::unique_lock<std::mutex>, NoLock<std::mutex>>;
	std::mutex coutMutex;

	cout << boolalpha;
	cout << __PRETTY_FUNCTION__ << endl;
	cout << "      thread id: " << this_thread::get_id() << endl;
	cout << "activate Before: " << activateBefore << endl;
	cout << " throwException: " << throwException << endl;
	cout << "           join: " << join << endl;
	cout << "           lock: " << coutLock << endl;
	cout << "    numThreads : " << numThreads << endl;

	auto delay = 100ms;

	auto lambda = [&coutMutex, delay](auto future){
		{
			Lock lk(coutMutex);
			cout << endl<< __PRETTY_FUNCTION__  << endl;
			cout  << "thread id: " << this_thread::get_id() << endl;
		}
		this_thread::sleep_for(delay);
#ifdef CATCHEXCEPTION // demo system behavior if you don�t catch it
		try{
#endif
			{
				Lock lk(coutMutex);
				cout << endl << "before future.get() " << "thread id: " << this_thread::get_id() << endl;
			}
//			future.wait(); // #1
			bool joined = future.get(); // #1

			Lock lk(coutMutex);
			cout << "after future.get() joined: " << joined;
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
	};


	// create ThreadRAII before promise!
	// But this is not acquisition is initialization!
	// promise will be deleted before tr
	// in case of exception, future.get() will be interrupted via exception
	// but future.wait() not!
	vector<TRAII> threads(numThreads);
	std::promise<bool> promise;
	auto sharedFuture = promise.get_future().share();

	// initialize ThreadRAII
	for(auto &tr : threads)
		tr.set(std::thread(lambda, sharedFuture));

	if(activateBefore)
		this_thread::sleep_for(delay - 100ms); // activate before
	else
		this_thread::sleep_for(delay + 100ms); // activate after


	// if here happens an exception
	if(throwException)
		throw std::logic_error("Demo broken promise");

	cout << "before p.set_value() " << endl;
	promise.set_value(true); // #2
	{
		Lock lk(coutMutex);
		cout << "after p.set_value() " << endl;
	}
}





