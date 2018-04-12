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

template<typename Mutex>
struct NoLock{
	NoLock(Mutex& m){}
};

#define CATCHEXCEPTION

void demoPromiseSharedFuture(){
	// different scenarios
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

	// create ThreadRAII before promise!
	// But this is not acquisition is initialization!
	// promise will be deleted before tr
	// in case of exception, future.get() will be interrupted via exception
	// but future.wait() not!
	vector<TRAII> threads(numThreads);

	std::promise<bool> promise;

	auto sharedFuture = promise.get_future().share();

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



void demoPromiseSharedFuturePromiseRAII(){
	// experimental
	//RAII for Promise
	struct SetPromiseFalse{
		SetPromiseFalse(std::promise<bool>& p) : p(std::addressof(p)){}
		~SetPromiseFalse(){
			cout << hex;
			cout << __PRETTY_FUNCTION__ << endl;

			if(p)
				p->set_value(false);
		}
		void reset(){ p = nullptr; }
		std::promise<bool>* p;
	};

	// different scenarios
	const unsigned int numThreads = std::thread::hardware_concurrency();

	bool activateBefore = true;
	bool throwException = true;

	constexpr bool join = true;
	using Policy = std::conditional_t<join, JoinPolicy, DetachPolicy>;
	using TRAII = ThreadRAII<Policy>;

	constexpr bool lock = false;
	using Lock = std::conditional_t<lock, std::unique_lock<std::mutex>, NoLock<std::mutex>>;
	std::mutex coutMutex;

	std::promise<bool> p;
	using Future = decltype(p.get_future().share());

	auto sf = p.get_future().share();


	cout << boolalpha;
	cout << __PRETTY_FUNCTION__ << endl;
	cout << "      thread id: " << this_thread::get_id() << endl;
	cout << "activate Before: " << activateBefore << endl;
	cout << " throwException: " << throwException << endl;
	cout << "           lock: " << lock << endl;
	cout << "           join: " << join << endl;
	cout << "    numThreads : " << numThreads << endl;

	auto delay = 100ms;

	auto lambda = [&coutMutex, delay](Future f){
		this_thread::sleep_for(delay);
		{
			Lock lk(coutMutex);
			cout << "before f.get() " << "thread id: " << this_thread::get_id() << " " << __PRETTY_FUNCTION__  << endl;
		}
		try{
			bool joined = f.get(); // #1

			Lock lk(coutMutex);
			cout<< "after f.get() joined: " << joined
				<< " thread id: " << this_thread::get_id()
				<< endl;
		}catch(std::exception& e){
			Lock lk(coutMutex);
			cout << "lambda catch(std::exception&): " << e.what() << " thread id: " << this_thread::get_id()<< endl;
		}catch(...){
			Lock lk(coutMutex);
			cout << "lambda catch(...): " << " thread id: " << this_thread::get_id()<< endl;
		}
	};

	vector<TRAII> threads;
	for(unsigned int i=0; i<numThreads; ++i){
		threads.emplace(threads.end(), thread(lambda, sf));
	}

	if(activateBefore)
		this_thread::sleep_for(delay - 100ms); // notifiy before
	else
		this_thread::sleep_for(delay + 100ms); // notifiy after

	// start new block
	{
		SetPromiseFalse sp(p);

		// if here happens an exception
		if(throwException)
			throw std::logic_error("Demo broken promise");

		sp.reset();
//		try{
//			if(throwException)
//				throw std::logic_error("Demo broken promise");
//		}catch(...){
//	//		p.set_value(false);
//			throw;
//		}
	}
	cout << "before p.set_value() " << endl;
	p.set_value(true); // #2
	{
		Lock lk(coutMutex);
		cout << "after p.set_value() " << endl;
	}
}


