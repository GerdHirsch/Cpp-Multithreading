/*
 * Tasks.h
 *
 *  Created on: 15.04.2018
 *      Author: Gerd
 */

#ifndef TASKS_H_
#define TASKS_H_

#include <mutex>
#include <future>


namespace Demo{
template<typename Mutex>
struct NoLock{
	NoLock(Mutex& m){}
};
}
template<bool coutLock = true>
void task0(std::mutex& coutMutex, std::chrono::milliseconds delay, std::shared_future<void> future);

void task1(std::shared_future<bool> future);


#endif /* TASKS_H_ */
