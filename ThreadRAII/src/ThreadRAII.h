/*
 * ThreadRAII.h
 *
 *  Created on: 05.04.2018
 *      Author: Gerd
 */

#ifndef SRC_THREADRAII_H_
#define SRC_THREADRAII_H_
#include <thread>
#include <iostream>

struct JoinPolicy{
	void joinOrDetach(std::thread& t){
		t.join();
	}
};
struct DetachPolicy{
	void joinOrDetach(std::thread& t){
		t.detach();
	}
};
template<typename JoinOrDetachPolicy=JoinPolicy>
class ThreadRAII : public JoinOrDetachPolicy {
public:
	ThreadRAII(std::thread&& threadToManage) : threadToManage(std::move(threadToManage)){
//		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
	~ThreadRAII(){
//		std::cout << __PRETTY_FUNCTION__ << std::endl;

		if(threadToManage.joinable())
			this->joinOrDetach(threadToManage);
	}
	operator std::thread&(){ return threadToManage; }
private:
	std::thread threadToManage;
};



#endif /* SRC_THREADRAII_H_ */
