/*
 * BlockingQueue.h
 *
 *  Created on: 03.02.2018
 *      Author: Gerd
 */

#ifndef INCLUDE_BLOCKINGQUEUE_BLOCKINGQUEUE_H_
#define INCLUDE_BLOCKINGQUEUE_BLOCKINGQUEUE_H_

#include <queue>
#include <condition_variable>
namespace gh{

template<class Message>
class BlockingQueue{
	std::queue<Message> queue;
	std::condition_variable cv;
	std::mutex mutex;
public:
	void pop(Message& message){
		std::unique_lock<std::mutex> lock{mutex};
		auto pred = [=]{ return !queue.empty();};

		while(queue.empty()){
			cv.wait(lock, pred);
		}
		message = std::move(queue.front());
		queue.pop();
	}

	[[nodiscard]]
	bool popNonBlocking(Message& message){
		bool retVal{false};

		std::unique_lock<std::mutex> lock{mutex};
		if(!queue.empty()){
			message = std::move(queue.front());
			queue.pop();
		}
		return retVal;
	}
	void push(Message&& message){
		std::unique_lock<std::mutex> lock{mutex};
		queue.push(std::move(message));
		cv.notify_one();
	}
	void push(Message const & message){
		std::unique_lock<std::mutex> lock{mutex};
		queue.push(message);
		cv.notify_one();
	}
};

} // namespace gh


#endif /* INCLUDE_BLOCKINGQUEUE_BLOCKINGQUEUE_H_ */
