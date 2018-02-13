/*
 * DemoThreadLocal.h
 *
 *  Created on: 10.02.2018
 *      Author: Gerd
 */

#ifndef DEMOTHREADLOCAL_H_
#define DEMOTHREADLOCAL_H_

#include <iostream>
#include <thread>
#include <atomic>

struct Counter{
	std::atomic<int> counter;
	int threadNum;
	Counter() :
			counter { 0 }, threadNum { 0 } {
		std::cout << __PRETTY_FUNCTION__ << "thread: " << threadNum << std::endl;
	}
	Counter(int i) :
			counter { i }, threadNum { i } {
		std::cout << __PRETTY_FUNCTION__ << " i: " << counter << std::endl;
	}
	~Counter() {
		std::cout << __PRETTY_FUNCTION__ << " threadNum: " << threadNum << std::endl;
	}
};

std::ostream& operator<<(std::ostream& out, Counter const& counter){
	out << counter.counter;
	return out;
}

struct DemoThreadLocal{
	static constexpr std::size_t MAXTHREADS = 2;
	Counter *localCounts[MAXTHREADS]; //

	DemoThreadLocal()
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
		for(auto& p : localCounts) p = nullptr;
	}
	void demoThreadLocal(int threadNo){
		std::cout << __PRETTY_FUNCTION__ << " thread: " << threadNo <<  std::endl;
		Counter thread_local scopeLocal{threadNo};

		std::atomic_thread_fence( std::memory_order_acquire);

		localCounts[threadNo] = &scopeLocal;

		std::atomic_thread_fence( std::memory_order_release);
	}
	void resetLocals(){
		localCounts[0]->counter.store(-1, std::memory_order_relaxed);
		localCounts[1]->counter.store(-1, std::memory_order_relaxed);
	}
	void printThreadLocals(){
		std::cout << __PRETTY_FUNCTION__ << std::endl;

		for(Counter** p = &localCounts[0]; p < &localCounts[MAXTHREADS]; ++p)
			if(*p) std::cout << (*p)->counter.load(std::memory_order_relaxed) << std::endl;
			else std::cout << "No Pointer set" << std::endl;
	}
};



#endif /* DEMOTHREADLOCAL_H_ */
