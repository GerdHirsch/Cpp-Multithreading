/*
 * DemoPromiseRAII.cpp
 *
 *  Created on: 15.04.2018
 *      Author: Gerd
 */

#include "PromiseRAII.h"

#include <type_traits>
#include <future>
#include <thread>
#include <iostream>
using namespace std;

void demoPromiseRAII(){
	// different scenarios
#define CATCHEXCEPTION
	bool activateBefore = true;
	bool throwException = true;

	cout << boolalpha;
	cout << __PRETTY_FUNCTION__ << endl;
	cout << "      thread id: " << this_thread::get_id() << endl;
	cout << "activate Before: " << activateBefore << endl;
	cout << " throwException: " << throwException << endl;

	auto delay = 100ms;

	auto lambda0 =
	[delay](auto future){
			cout << endl
				 << __PRETTY_FUNCTION__ << endl;
			cout << "thread id: " << this_thread::get_id() << endl;
			this_thread::sleep_for(delay);
			cout << "before future access " << endl;

#ifdef CATCHEXCEPTION // demo system behavior if you don´t catch it
			try{
#endif
				future.get(); // #1 löst exception aus
				cout << endl << "after future access " << endl;
#ifdef CATCHEXCEPTION
			}catch(std::future_error& e){
				cout << endl << "lambda catch: " << e.what() << endl;
			}
#endif
		};


	std::promise<void> p;
	auto latchedTask = makeLatchedTasks(std::move(p), lambda0);


	if(activateBefore)
		this_thread::sleep_for(delay - delay); // notifiy before wait
	else
		this_thread::sleep_for(delay + delay); // notifiy after wait

	// if here happens an exception
	if(throwException)
		throw std::logic_error("Demo broken promise");

	cout << endl << "before promise.set_value() " << endl;
//	p.set_value(); // #2
	latchedTask.getPromise().set_value(); // #2
	cout << endl << "after promise.set_value() " << endl;
}


namespace Demo{
template<typename Mutex>
struct NoLock{
	NoLock(Mutex& m){}
};

void task(std::shared_future<bool> future){
	cout << __PRETTY_FUNCTION__ << endl;
	cout << endl << "before future.wait() " << "thread id: " << this_thread::get_id() << endl;
	future.wait();
	cout << "after future.wait()\n";
}
}



void demoPromiseRAIISharedFuture(){
	// different scenarios
// ugly works only for demo purposes
#define CATCHEXCEPTION
	const unsigned int numThreads = std::thread::hardware_concurrency();

	bool activateBefore{false};
	bool throwException{false};

	constexpr bool coutLock = false;
	using Lock = std::conditional_t<coutLock, std::unique_lock<std::mutex>, Demo::NoLock<std::mutex>>;
	std::mutex coutMutex;

	cout << boolalpha;
	cout << __PRETTY_FUNCTION__ << endl;
	cout << "      thread id: " << this_thread::get_id() << endl;
	cout << "activate Before: " << activateBefore << endl;
	cout << " throwException: " << throwException << endl;
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
#ifdef CATCHEXCEPTION // demo system behavior if you don´t catch it
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
	std::promise<bool> promise;
	auto latchedTasks = makeLatchedTasks(std::move(promise), lambda, Demo::task);

	if(activateBefore)
		this_thread::sleep_for(delay - 100ms); // activate before
	else
		this_thread::sleep_for(delay + 100ms); // activate after


	// if here happens an exception
	if(throwException)
		throw std::logic_error("Demo broken promise");

	cout << "before p.set_value() " << endl;
	latchedTasks.getPromise().set_value(true); // #2
	{
		Lock lk(coutMutex);
		cout << "after p.set_value() " << endl;
	}
}

