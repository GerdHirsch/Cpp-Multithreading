/*
 * main.cpp
 *
 *  Created on: 05.04.2018
 *      Author: Gerd
 *  Quelle: effective modern C++, Item 36
 */

#include "reallyAsync.h"

#include <future>
#include <thread>
#include <iostream>
using namespace std;

//#include <chrono> // f�r s, ms, ... wird nicht ben�tig
//using namespace std::literals;

//---------------------------------------------------------------------
void myTask(){
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	std::this_thread::sleep_for(1'000ms); //1'000'000 separators C++14
}
//---------------------------------------------------------------------
void demoDeferedProblem(){
	std::cout << __PRETTY_FUNCTION__ << std::endl;

	// zum experimentieren
//	std::launch launchPolicy = std::launch::async;
	std::launch launchPolicy = std::launch::async | std::launch::deferred; // default
	auto future = std::async(launchPolicy, myTask);

	while(future.wait_for(100ms) != std::future_status::ready){
		std::cout << "future not ready!" << std::endl;
	}
	std::cout << "future ready!" << std::endl;
}
//---------------------------------------------------------------------
/**
 * param: future
 * return: std::future_status of future
 * future.getStatus() exisitiert nicht darum
 * ein dummy aufruf future.wait_for(0s)
 * if(future.wait_for(0s) == std::future_status::deferred)
 * mit einem Namen versehen: getStatus(future)
 */
template<typename ReturnType>
std::future_status getStatus(std::future<ReturnType> const& future){
	return future.wait_for(0s);
}
void demoDeferedSolution(){
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	std::launch launchPolicy = std::launch::async;
	auto future = std::async(launchPolicy, myTask);
//	auto future = std::async(myTask); // default

	if(getStatus(future) == std::future_status::deferred){
		std::cout << "future defered!" << std::endl;
		future.wait();
		std::cout << "future defered ready!" << std::endl;
	} else {
		while(future.wait_for(100ms) != std::future_status::ready){
			std::cout << "future async not ready!" << std::endl;
		}
		std::cout << "future async ready!" << std::endl;
	}
}

void demoReallyAsync(){
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	auto future = reallyAsync(myTask);

	if(getStatus(future) == std::future_status::deferred){
		std::cout << "future defered!" << std::endl;
		future.wait();
		std::cout << "future defered ready!" << std::endl;
	} else {
		while(future.wait_for(100ms) != std::future_status::ready){
			std::cout << "future not ready!" << std::endl;
		}
		std::cout << "future ready!" << std::endl;
	}
}
//---------------------------------------------------------------------
void demoIgnoreFuture(){
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	auto future = reallyAsync(myTask);

	return;
}

//---------------------------------------------------------------------

//---------------------------------------------------------------------
void demoSharedFuture();
int main(){
	cout << "DemoAsync" << endl;
//	demoDeferedProblem();
//	demoDeferedSolution();
//	demoReallyAsync();
//	demoIgnoreFuture();
	demoSharedFuture();
}


