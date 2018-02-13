#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>

using namespace std;

bool sleep = false;

bool ready{false};
int number{-1};
std::atomic<bool> readyAtomic{false};

//static std::memory_order mo_load = std::memory_order_acquire;
//static std::memory_order mo_store = std::memory_order_release;
static std::memory_order mo_load = std::memory_order_relaxed;
static std::memory_order mo_store = std::memory_order_relaxed;

void taskAtomicFunction(){
	int count = 0;
	while( !readyAtomic.load(mo_load) ) {
//		std::this_thread::yield();
		++count;
	}
	cout << "count: " << count << endl;
	cout << "number: " << number << endl;
}
void demoAtomic(){
	cout << "demoAtomic()" << endl;
	thread t1(taskAtomicFunction);

	number = 42;

	if(sleep) std::this_thread::sleep_for(std::chrono::nanoseconds(1));

	readyAtomic.store(true, mo_store);
	t1.join();
}

void taskNonAtomicFunction(){
	int count = 0;
	while( !ready ) {
//		std::this_thread::yield();
		++count;
	}
	cout << "count: " << count << endl;
	cout << "number: " << number << endl;
}

void demoNonAtomic(){
	cout << "demoNonAtomic()" << endl;
	thread t1(taskNonAtomicFunction);

	number = 42;

	if(sleep) std::this_thread::sleep_for(std::chrono::nanoseconds(1));

	ready = true;
	t1.join();
}
int main(){

	cout << "StaleData" << endl;
	demoNonAtomic();
	demoAtomic();

}
