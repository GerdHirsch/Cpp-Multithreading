/*
 * main.cpp
 *
 *  Created on: 10.02.2018
 *      Author: Gerd
 *  demo Deklaration, Lebensdauer
 *  thread_local deklarierte Variablen und
 *  Zugriff aus verschiedenen Threads auf
 */

#include "DemoThreadLocal.h"

#include <vector>
#include <iostream>
#include <chrono>
using namespace std;

void f(DemoThreadLocal& dtl, unsigned int threadNum){
	cout
		<< " === "
		<< __PRETTY_FUNCTION__
		<< " dtl.demoThreadLocal(" << threadNum << ");"
		<< endl;
	dtl.demoThreadLocal(threadNum);
	while(dtl.localCounts[threadNum]->counter.load(std::memory_order_relaxed) != -1)
		;
	cout << "end " << __PRETTY_FUNCTION__ << endl;

}

int main(){
	cout << "ThreadLocal" << endl;

	{
		cout << "=== main: DemoThreadLocal dtl;" << endl;
		DemoThreadLocal dtl;
		cout << "=== main: dtl.demoThreadLocal(0);" << endl;
		dtl.demoThreadLocal(0);

		cout << "=== main: dtl.printThreadLocals();" << endl;
		dtl.printThreadLocals();


		cout << endl << endl;

		vector<thread> threads;
		for(unsigned int i = 1; i < DemoThreadLocal::MAXTHREADS; ++i){
			cout << "=== main: std::thread t(f, std::ref(dtl));" << endl;
			threads.emplace_back(f, std::ref(dtl), i );
		}
		cout << "=== main: sleep_for(chrono::milliseconds(10));" << endl;
		this_thread::sleep_for(chrono::milliseconds(10));

		cout << "=== main: dtl.printThreadLocals();" << endl;
		dtl.printThreadLocals();

		cout << "=== main: dtl.resetLocals();" << endl;
		dtl.resetLocals();
//		dtl.printThreadLocals();

		for(auto& t : threads){
			cout << "=== main: t.join();" << endl;
			t.join();
		}
	}
	cout << "end ThreadLocal" << endl;
}


