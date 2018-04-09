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

	bool notifyBeforeWait = false;
	auto duration = 100ms;

	std::thread t
	(
		[&dcv, duration]{


			this_thread::sleep_for(duration);

			std::unique_lock<std::mutex> lk(dcv.m);

			cout << "before cv.wait(lk) " << __PRETTY_FUNCTION__ << "thread id: " << this_thread::get_id() << endl;
			dcv.cv.wait(lk,[&dcv]{return dcv.b;} ); // #1
			cout << "after cv.wait(lk) " <<  endl;
	});

	if(notifyBeforeWait)
		this_thread::sleep_for(duration - 100ms); // notifiy before wait
	else
		this_thread::sleep_for(duration + 100ms); // notifiy after wait

	dcv.b = true;

	cout << "before cv.notify_one() " << endl;
	dcv.cv.notify_one(); // #2
	cout << "after cv.notify_one() " << endl;

	t.join();
}


