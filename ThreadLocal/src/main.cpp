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

#include <iostream>
#include <chrono>
using namespace std;

void f(DemoThreadLocal& dtl){
	cout
		<< " === "
		<< __PRETTY_FUNCTION__
		<< " dtl.demoThreadLocal(0);"
		<< endl;
	dtl.demoThreadLocal(1);
	while(dtl.localCounts[1]->counter != -1)
		;
	cout << "end "<< __PRETTY_FUNCTION__ << endl;

}

int main(){
	cout << "ThreadLocal" << endl;

	{
		cout << "=== main: DemoThreadLocal dtl;" << endl;
		DemoThreadLocal dtl;
		cout << "=== main: dtl.demoThreadLocal(0);" << endl;
		dtl.demoThreadLocal(0);

		dtl.printThreadLocals();

		cout << "=== main: std::thread t(f, std::ref(dtl));" << endl;
		std::thread t(f, std::ref(dtl));

		this_thread::sleep_for(chrono::milliseconds(10));

		cout << "=== main: dtl.printThreadLocals();" << endl;
		dtl.printThreadLocals();
		cout << "=== main: dtl.resetLocals();" << endl;
		dtl.resetLocals();
		dtl.printThreadLocals();
		t.join();
	}
	cout << "end ThreadLocal" << endl;
}


