/*
 * DemoPromiseFuture.cpp
 *
 *  Created on: 09.04.2018
 *      Author: Gerd
 */

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

void demoPromiseFuture(){
	cout << __PRETTY_FUNCTION__ << "thread id: " << this_thread::get_id() << endl;

	bool notifyBeforeWait = true;
	auto duration = 100ms;

	std::promise<void> p;
	using Future = decltype(p.get_future());

	std::thread t([duration](Future&& f){

		this_thread::sleep_for(duration);

		cout << "before f.wait() " << __PRETTY_FUNCTION__ << "thread id: " << this_thread::get_id() << endl;
		f.wait(); // #1
		cout << "after f.wait() " << endl;
		}, p.get_future()

	);

	if(notifyBeforeWait)
		this_thread::sleep_for(duration - 100ms); // notifiy before wait
	else
		this_thread::sleep_for(duration + 100ms); // notifiy after wait

	cout << "before p.set_value() " << endl;
	p.set_value(); // #2
	cout << "after p.set_value() " << endl;

	t.join();
}

template<typename Mutex>
struct NoLock{
	NoLock(Mutex& m){}
};

void demoPromiseFutureMultipleThreads(){
	bool activateBeforeWait = false;
//	using Lock = std::unique_lock<std::mutex>;
	using Lock = NoLock<std::mutex>;
	std::mutex coutMutex;


	cout << boolalpha;
	cout << __PRETTY_FUNCTION__ << " thread id: " << this_thread::get_id() << endl;
	cout << "activate Before Wait: " << activateBeforeWait << endl;

	auto duration = 100ms;

	std::promise<void> p;
	using Future = decltype(p.get_future().share());
	auto lambda = [&coutMutex, duration](Future f){

		this_thread::sleep_for(duration);
		{
			Lock lk(coutMutex);
			cout << "before f.wait() " << "thread id: " << this_thread::get_id() << " " << __PRETTY_FUNCTION__  << endl;
		}
		f.wait(); // #1

		Lock lk(coutMutex);
		cout << "after f.wait() " << " thread id: " << this_thread::get_id() << endl;
		};

	auto sf = p.get_future().share();

	constexpr int numThreads = 4;
	vector<thread> threads;
	for(int i=0; i<numThreads; ++i)
		threads.push_back(thread(lambda, sf));
//		threads.emplace(lambda, sf);

	if(activateBeforeWait)
		this_thread::sleep_for(duration - 100ms); // notifiy before wait
	else
		this_thread::sleep_for(duration + 100ms); // notifiy after wait

	cout << "before p.set_value() " << endl;
	p.set_value(); // #2
	{
		Lock lk(coutMutex);
		cout << "after p.set_value() " << endl;
	}

	for(int i=0; i<numThreads; ++i)
		threads[i].join();
}


