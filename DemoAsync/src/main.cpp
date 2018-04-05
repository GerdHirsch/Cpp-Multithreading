/*
 * main.cpp
 *
 *  Created on: 05.04.2018
 *      Author: Gerd
 *  Quelle: effective modern C++, Item 36
 */

#include <future>
#include <thread>
#include <iostream>
using namespace std;

//#include <chrono>
//using namespace std::literals;

void myTask(){
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	std::this_thread::sleep_for(1s);
}

void demoDeferedProblem(){
	std::cout << __PRETTY_FUNCTION__ << std::endl;
//	std::launch launchPolicy = std::launch::async;
	std::launch launchPolicy = std::launch::async | std::launch::deferred; // default
	auto future = std::async(launchPolicy, myTask);

	while(future.wait_for(100ms) != std::future_status::ready){
		std::cout << "future not ready!" << std::endl;
	}
	std::cout << "future ready!" << std::endl;
}
void demoDeferedSolution(){
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	std::launch launchPolicy = std::launch::async;
	auto future = std::async(launchPolicy, myTask);

//	auto future = std::async(myTask); // default

	if(future.wait_for(0s) == std::future_status::deferred){
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

template<typename Task, typename... Args>
inline
auto // C++14 return type deduction
//std::future<typename std::result_of<Task(Args...)>::type> // return type C++11
reallyAsync(Task&& task, Args&&... args)
{
	return 	std::async
			(
					std::launch::async,
					std::forward<Task>(task),
					std::forward<Args>(args)...
			);
}
void demoReallyAsync(){
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	auto future = reallyAsync(myTask);

	if(future.wait_for(0s) == std::future_status::deferred){
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
int main(){
	cout << "DemoAsync" << endl;
//	demoDeferedProblem();
//	demoDeferedSolution();
	demoReallyAsync();
}


