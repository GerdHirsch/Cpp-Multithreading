/*
 * DemoConditionVariable.cpp
 *
 *  Created on: 09.04.2018
 *      Author: Gerd
 */
#include <condition_variable>
#include <iostream>
#include <thread>
using namespace std;

struct DemoConditionVariable{
	std::condition_variable cv;
	std::mutex m;
	bool b{false};
};
/**
 * demonstrates
 * if sender notifies cv (#2) befor receiver waits (#1)
 * receiver will hang
 * if there is a spurious wakeup (#1), receiver needs a way to check,
 * whether the wakeup is not spurious
 */
void demoConditionVariable(){
	cout << __PRETTY_FUNCTION__ << "thread id: " << this_thread::get_id() << endl;
	DemoConditionVariable dcv;

	bool notifyBeforeWait = true;
	auto duration = 100ms;

	std::thread t
	(
		[&dcv, duration]{


			this_thread::sleep_for(duration);
// cv.wait macht lock und unlock, das geht mit lock_guard nicht
//			std::lock_guard<std::mutex> lk(dcv.m);
			std::unique_lock<std::mutex> lk(dcv.m);

			cout << "before cv.wait(lk) " << __PRETTY_FUNCTION__ << "thread id: " << this_thread::get_id() << endl;
//			dcv.cv.wait(lk); // #1
			dcv.cv.wait(lk,[&dcv]{return dcv.b;} ); // #1
			cout << "after cv.wait(lk) " <<  endl;
	});

	if(!notifyBeforeWait)
		this_thread::sleep_for(duration + duration); // notifiy after wait

	{
		std::lock_guard<std::mutex> g(dcv.m);
		dcv.b = true;
	}
	cout << "after cv.notify_one() " << endl;
	dcv.cv.notify_one(); // #2
	cout << "before cv.notify_one() " << endl;

	t.join();
}


