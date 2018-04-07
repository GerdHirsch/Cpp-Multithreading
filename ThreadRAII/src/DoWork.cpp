/*
 * DoWork.cpp
 *
 *  Created on: 07.04.2018
 *      Author: Gerd
 */

#include <functional>
#include <thread>
#include <vector>
#include <iostream>
using namespace std;

//constexpr auto tenMillion = 10'000'000; //C++14 digit separator
constexpr auto tenMillion = 10; // for demo
using GoodValues = std::vector<int>;

bool conditionsAreSatisfied(){ return false; }

void  performComputation(GoodValues goodValues){
	// do something
}

bool doWork(std::function<bool(int)> filter, int maxValue = tenMillion){
	GoodValues goodValues;

	std::thread t([&filter, maxValue, &goodValues]{
		for(auto i = 0; i<= maxValue; ++i){
			if(filter(i)) goodValues.push_back(i);
		}
	});

	// demo: this is why we need to use std::thread directly
//	auto nh = t.native_handle();

	if(conditionsAreSatisfied()){
		t.join();
		performComputation(goodValues);
		return true;
	}
	// in this path of execution
	// no join or detach exists -> std::terminate
	// use ThreadRAII instead
	return false;

	// question: what should the default action be? join, detach?
	// join leads to performance anomalies
	// detach leads to a demon thread running long after this_thread, accessing dangling references (goodValues)
}
void demoDoWork(){
	doWork([](int i){ return true; });
}


