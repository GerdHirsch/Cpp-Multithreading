/*
 * ReallyAsync.h
 *
 *  Created on: 08.04.2018
 *      Author: Gerd
 */

#ifndef SRC_REALLYASYNC_H_
#define SRC_REALLYASYNC_H_
#include <future>
//---------------------------------------------------------------------
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

#endif /* SRC_REALLYASYNC_H_ */
