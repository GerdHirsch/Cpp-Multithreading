/*
 * DemoDifferentTasksSignatures.cpp
 *
 *  Created on: 15.04.2018
 *      Author: Gerd
 */



#include "Tasks.h"
#include "PromiseRAII.h"

#include <type_traits>
#include <chrono>
#include <future>
#include <thread>
#include <vector>
#include <iostream>
using namespace std;

void demoDifferentTasksSignatures(){
	// different scenarios
	const unsigned int numThreads = std::thread::hardware_concurrency();
	bool activateBefore{true};
	bool throwException{true};

	constexpr bool coutLock = true;
	using Lock = std::conditional_t<coutLock, std::unique_lock<std::mutex>, Demo::NoLock<std::mutex>>;
	std::mutex coutMutex;

	constexpr bool join{true};
	using Policy = std::conditional_t<join, JoinPolicy, DetachPolicy>;
	using TRAII = ThreadRAII<Policy>;

	cout << boolalpha;
	cout << __PRETTY_FUNCTION__ << endl;
	cout << "      thread id: " << this_thread::get_id() << endl;
	cout << "activate Before: " << activateBefore << endl;
	cout << " throwException: " << throwException << endl;
	cout << "           join: " << join << endl;
	cout << "           lock: " << coutLock << endl;
	cout << "    numThreads : " << numThreads << endl;

	auto delay = 100ms;

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
		tr.set(std::thread(task1, sharedFuture));

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
