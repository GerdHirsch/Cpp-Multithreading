/*
 main.cpp
 *
 *  Created on: 09.04.2018
 *      Author: Gerd
 *  Effective Modern C++, Item 39
 *  start threads suspended to allocate the needed resources and run the thread immediately
 *  wenn an event occurs
 *  https://stackoverflow.com/questions/3156852/why-would-i-want-to-start-a-thread-suspended
 *
 *  see also project StaleData for a loop based bad polling solution
 */

#include <thread>
#include <iostream>
using namespace std;

void demoConditionVariable();
void demoPromiseFutureSimple();
void demoPromiseFuture();
void demoPromiseSharedFuture();

void demoPromiseRAII();
void demoPromiseRAIISharedFuture();

int main(){
	cout << "ActivateAnotherTask" << endl;

	try{
//	demoPromiseFutureSimple();
//	cout << endl;
//	demoPromiseFuture();
//	cout << endl;
//	demoPromiseSharedFuture();
	cout << endl;
//	demoPromiseRAII();
	demoPromiseRAIISharedFuture();
//	cout << endl;
//	demoConditionVariable();
	}catch(std::exception& e){
		cerr << "=== main() CATCH: " << e.what() << endl;
	}
	this_thread::sleep_for(1s);
}


