/*
 * DemoPromiseFuture.cpp
 *
 *  Created on: 09.04.2018
 *      Author: Gerd
 */

#include <future>
#include <thread>
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

	bool notifyBeforeWait = true;
	if(notifyBeforeWait)
		this_thread::sleep_for(duration - 100ms); // notifiy before wait
	else
		this_thread::sleep_for(duration + 100ms); // notifiy after wait

	cout << "before p.set_value() " << endl;
	p.set_value(); // #2
	cout << "after p.set_value() " << endl;

	t.join();
}



