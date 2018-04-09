#include <future>
#include <thread>
#include <iostream>
#include <mutex>
#include <atomic>

using namespace std;

//#include <chrono> // für s, ms, ... wird nicht benötig
//using namespace std::chrono_literals;

bool sleep = true;
int number{-1};
//---------------------------------------------------------------------
std::atomic<bool> readyAtomic{false};

//static std::memory_order mo_load = std::memory_order_acquire;
//static std::memory_order mo_store = std::memory_order_release;
static std::memory_order mo_load = std::memory_order_relaxed;
static std::memory_order mo_store = std::memory_order_relaxed;

void taskAtomicFunction(){
	cout << __PRETTY_FUNCTION__ << endl;
	int count = 0;
	// polling is almost always a bad idea
	while( !readyAtomic.load(mo_load) ) {
//		std::this_thread::yield();
		++count;
	}
	cout << "count: " << count << endl;
	cout << "number: " << number << endl;
}
void demoAtomic(){
	cout << __PRETTY_FUNCTION__ << endl;
	thread t1(taskAtomicFunction);

	number = 42;

	if(sleep) std::this_thread::sleep_for(1ns);

	readyAtomic.store(true, mo_store);
	t1.join();
}
//---------------------------------------------------------------------
bool ready{false};

void taskNonAtomicFunction(){
	cout << __PRETTY_FUNCTION__ << endl;
	int count = 0;
	// polling is almost always a bad idea
	while( !ready ) {
		std::this_thread::yield();
		++count;
	}
	cout << "count: " << count << endl;
	cout << "number: " << number << endl;
}

void demoNonAtomic(){
	cout << __PRETTY_FUNCTION__ << endl;
	thread t1(taskNonAtomicFunction);

	number = 42;

	if(sleep) std::this_thread::sleep_for(1ns);

	ready = true;
	t1.join();
}
//---------------------------------------------------------------------
int main(){
	cout << "StaleData" << endl;
	demoNonAtomic();
//	demoAtomic();
}
