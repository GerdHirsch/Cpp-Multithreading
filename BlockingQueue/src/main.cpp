/*
 * main.cpp
 *
 *  Created on: 03.02.2018
 *      Author: Gerd
 */

#include "../include/NonBlockingQueue.h"
#include <thread>
#include <chrono>
#include <functional>
#include <iostream>
#include "../include/BlockingQueue/BlockingQueue.h"

using namespace std;
using namespace std::chrono;

using Message = std::size_t;
using BQueue = gh::BlockingQueue<Message>;

/*
 * 1000: 20 ms passed
 * 10000: 143 ms passed
 * 100000: 2917 ms passed
 * 1000000: 29149 ms passed
 */

void consumer(BQueue& bQueue, std::size_t maxMessages){
	std::vector<Message> messages(maxMessages);

	Message message{};
//	cout << endl << "consumer this_thread::get_id(): " << this_thread::get_id() << endl;
//	this_thread::sleep_for(chrono::milliseconds{1000});

	auto t0 = high_resolution_clock::now();
	do{
//		cout << " C vor pop ";
		bQueue.pop(message);
		messages.push_back(message);
//		cout << " C: " << message;
//		if(message == 2)
//			this_thread::sleep_for(chrono::milliseconds{150});
	}while(message);

	auto t1 = high_resolution_clock::now();
//
//	cout << duration_cast<nanoseconds>(t1 - t0).count() <<" ns passed" << endl;
	cout << maxMessages << ": " << duration_cast<milliseconds>(t1 - t0).count() <<" ms passed" << endl;
}
void producer(BQueue& bQueue, std::size_t maxMessages){
//	cout << endl << "producer this_thread::get_id(): " << this_thread::get_id() << endl;
//	size_t maxMessages = 10000;
	for(Message message = 1; message<maxMessages; ++message){
//		cout << " P: " << message;
		bQueue.push(message);
//		this_thread::sleep_for(chrono::milliseconds{50});
	}
	bQueue.push(0);
}
int main(){
	cout << "BlockingQueue" << endl;
	cout << endl << "main this_thread::get_id(): " << this_thread::get_id() << endl;
	BQueue bQueue{};

	for(auto i = 1; i<10000; i*=10){
		std::size_t maxMessages = 1000 * i;

		thread c(consumer, std::ref(bQueue), maxMessages);
		thread p(producer, std::ref(bQueue), maxMessages);

		c.join();
		p.join();
	}
	cout << endl << "end main" << endl;
}


