/*
 * PromiseRAII.h
 *
 *  Created on: 15.04.2018
 *      Author: Gerd
 */

#ifndef PROMISERAII_H_
#define PROMISERAII_H_

#include "ThreadRAII.h"

#include <future>
#include <iterator>
#include <vector>

/**
auto p = makeLatchedTasks(std::promise<bool>, task1, task2);
...
p.set_setValue(true;)
*/
template<typename Promise>
class LatchedTasksRAII{

public:
	template<typename... Tasks>
//	LatchedTasksRAII(std::promise<void>&& p, Tasks... tasks)
	LatchedTasksRAII(Promise&& p, Tasks... tasks)
	:
	  threads(sizeof...(tasks)),
	  promise(std::move(p))
	{
		auto future = promise.get_future().share();
		Iterator it = threads.begin();
		insertThread(it, future, tasks...);
	}
	auto& getPromise() { return promise; }
private:
	using TRAII = ThreadRAII<JoinPolicy>;
	using Threads = std::vector<TRAII>;
	using Iterator = Threads::iterator;
	using Future = decltype(std::declval<Promise>().get_future().share());

	template<typename Task, typename... Tasks>
	void insertThread(Iterator it, Future future, Task task, Tasks... tasks){
		(*it).set(std::thread(task, future));
		insertThread(++it, future, tasks...);
	}
	template<typename Task>
	void insertThread(Iterator it, Future future, Task task){
		(*it).set(std::thread(task, future));
	}

	Threads threads;
	Promise promise;
};

//---------------------------------------------------------------------
template<typename Promise>
class LatchedTaskRAII{
public:
	template<typename Task>
	LatchedTaskRAII(Promise&& p, Task t)
	:	promise(std::move(p))
	{
		threadRAII.set(std::thread(t, promise.get_future()) );
	}
	auto& getPromise() { return promise; }
private:
	ThreadRAII<JoinPolicy> threadRAII;
	Promise promise;
};

//---------------------------------------------------------------------
template<typename ReturnValue, typename... Tasks>
auto makeLatchedTasks(std::promise<ReturnValue>&& p, Tasks... tasks){

	return LatchedTasksRAII<std::promise<ReturnValue>>(std::move(p), tasks...);
}
//---------------------------------------------------------------------
template<typename ReturnValue, typename Task>
auto makeLatchedTasks(std::promise<ReturnValue>&& p, Task task){

	return LatchedTaskRAII<std::promise<ReturnValue>>(std::move(p), task);
}
//---------------------------------------------------------------------

#endif /* PROMISERAII_H_ */
