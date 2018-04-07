/*
 * main.cpp
 *
 *  Created on: 05.04.2018
 *      Author: Gerd
 *
 *  Effective Modern C++, Item 37
 *  things to remember:
 *  Make threads unjoinable on all paths
 *  join-on-desturction can lead to difficult-to-debug performance anomalies
 *  detach-on-destruction can lead to difficult-to-debug undefined behavior
 *  declare thread objects last in lists of data members ThreadRAII
 */

#include "ThreadRAII.h"

#include <iostream>
using namespace std;

struct MyTask{
	static int id;
	MyTask():taskId(++id){
		std::cout << __PRETTY_FUNCTION__ << "id: " << taskId << std::endl;
	}
	~MyTask(){
		std::cout << __PRETTY_FUNCTION__ << "id: " << taskId << std::endl;
	}
	MyTask(MyTask const& rhs):taskId(rhs.taskId+1){
		std::cout << __PRETTY_FUNCTION__ << "id: " << taskId << std::endl;
	}
	MyTask(MyTask && rhs):taskId(rhs.taskId+1){
		std::cout << __PRETTY_FUNCTION__ << "id: " << taskId << std::endl;
	}
	int sum;
	int operator()(){
		std::cout << __PRETTY_FUNCTION__ << std::endl;

		sum = 0;
		for(int i=0; i<3; ++i){
			std::cout << "i: " << i << std::endl;
			sum += i;
			std::this_thread::sleep_for(10ms);
		}
		return sum;
	}
	int taskId;
};
int MyTask::id = 0;

void demoThreadRAIIJoin(){
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	{
		// most vexing parse of C++!!!
		// interestingly, we need only on pair of parenthesis
		// around ( std::thread(..) ) but not around MyTask() as below
		ThreadRAII<> tr0( ( std::thread( MyTask()) ) );

		// that works
//		MyTask myTask;
//		std::thread t0(myTask);
//		t0.join();

		// here we need an extra parenthesis around ( MyTask() )
		// else t1 is a function returning std::thread and expecting a MyTask() what?
//		std::thread t1( MyTask() );
//		t1.join();
	}
	std::cout << "end of " << __PRETTY_FUNCTION__ << std::endl;
}

void demoThreadRAIIDetach(){
	std::cout << __PRETTY_FUNCTION__ << std::endl;

	ThreadRAII<DetachPolicy> t( ( std::thread(MyTask()) ));

	std::cout << "end of " << __PRETTY_FUNCTION__ << std::endl;

}

void demoThreadRAIIJoinByReference(){
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	MyTask myTask;
	ThreadRAII<> tr( (std::thread(std::ref(myTask))) );

	// this is a race condition!!! -> non synchronized access to shared data
	// result depends on order of execution
	std::cout << "sum: " << myTask.sum << std::endl;

}

void demoThreadRAIIMove(){
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	MyTask myTask;
	ThreadRAII<> tr0( (std::thread(std::ref(myTask))) );

	ThreadRAII<> tr1(std::move(tr0));

//	static_cast<std::thread&>(tr1).join();
//	std::thread& t = tr1;
//	t.join();
	tr1.get().join();
}

void demoDoWork();

int main(){
	cout << "ThreadRAII" << endl;

//	demoThreadRAIIJoin();
//	demoThreadRAIIJoinByReference();
//	demoThreadRAIIDetach();
//	demoThreadRAIIMove();

	//  terminates process!
	demoDoWork();

	std::this_thread::sleep_for(5s);
	cout << "End of ThreadRAII" << endl;
}


