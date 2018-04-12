/*
 * DemoPromiseFuture.cpp
 *
 *  Created on: 09.04.2018
 *      Author: Gerd
 */

#include "ThreadRAII.h"

#include <type_traits>
#include <future>
#include <thread>
#include <vector>
#include <iostream>
using namespace std;

/**
 * demonstrates
 * if sender set_value (#2) befor receiver waits (#1)
 * receiver will not hang
 * there can´t be a spurious wakeup (#1)
 * no extra bool flag is needed
 * just simple and straight forward
 */

void demoPromiseFutureSimple(){
	bool activateBefore = true;
	auto duration = 100ms;

	cout << boolalpha;
	cout << __PRETTY_FUNCTION__ << " thread id: " << this_thread::get_id() << endl;
	cout << "activate Before: " << activateBefore << endl;


	std::promise<void> p;
	using Future = decltype(p.get_future());

	std::thread t([duration](Future&& f){

		this_thread::sleep_for(duration);

		cout << "before f.wait() " << __PRETTY_FUNCTION__ << "thread id: " << this_thread::get_id() << endl;
		f.wait(); // #1
		cout << "after f.wait() " << endl;
		}, p.get_future()
	);

	if(activateBefore)
		this_thread::sleep_for(duration - 100ms); // notifiy before wait
	else
		this_thread::sleep_for(duration + 100ms); // notifiy after wait

	//throw exception and than what
	// see http://scottmeyers.blogspot.de/2013/12/threadraii-thread-suspension-trouble.html
	// and Effective Modern C++ Item 39

	cout << "before p.set_value() " << endl;
	p.set_value(); // #2
	cout << "after p.set_value() " << endl;

	t.join();
}

#define CATCHEXCEPTION

void demoPromiseFuture(){
	// different scenarios
	bool activateBefore = true;
	bool throwException = true;
	constexpr bool join{false};
	using Policy = std::conditional_t<join, JoinPolicy, DetachPolicy>;

	cout << boolalpha;
	cout << __PRETTY_FUNCTION__ << endl;
	cout << "      thread id: " << this_thread::get_id() << endl;
	cout << "activate Before: " << activateBefore << endl;
	cout << " throwException: " << throwException << endl;
	cout << "           join: " << join << endl;

	auto delay = 100ms;

	// create ThreadRAII before promise!
	// But this is not acquisition is initialization!
	// promise will be deleted before tr
	// in case of exception, future.get() will be interrupted via exception
	// but future.wait() not!
	ThreadRAII<Policy> tr;

//	std::promise<bool> p;
	std::promise<void> p;

	auto lambda =
	[delay](auto future){
			cout << endl
				 << __PRETTY_FUNCTION__ << endl;
			cout << "thread id: " << this_thread::get_id() << endl;
			this_thread::sleep_for(delay);
			cout << "before future access " << endl;

#ifdef CATCHEXCEPTION // demo system behavior if you don´t catch it
			try{
#endif
//				future.wait(); // #1 löst keine exception aus
				future.get(); // #1 löst exception aus
				cout << endl << "after future access " << endl;

				// for promise<bool>
//				bool taskCanRun = future.get(); // #1
//				cout << endl << "after future.get() taskCanRun: " << taskCanRun << endl;

#ifdef CATCHEXCEPTION
			}catch(std::future_error& e){
				cout << endl << "lambda catch: " << e.what() << endl;
			}
#endif
		};

	// initialize ThreadRAII
	tr.set(std::thread(lambda, p.get_future()) );

	if(activateBefore)
		this_thread::sleep_for(delay - 100ms); // notifiy before wait
	else
		this_thread::sleep_for(delay + 100ms); // notifiy after wait

	// if here happens an exception
	if(throwException)
		throw std::logic_error("Demo broken promise");

	cout << endl << "before p.set_value() " << endl;
//	p.set_value(true); // #2
	p.set_value(); // #2
	cout << endl << "after p.set_value() " << endl;
}

