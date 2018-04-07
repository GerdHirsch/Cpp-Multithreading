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
// Alternativ zu JoinOrDetachPolicy eine enum JoinOrDetach{ join, detach }
// und einen Ctor(..., JoinOrDetach jod); see Item 37 S.254
template<typename JoinOrDetachPolicy=JoinPolicy>
class ThreadRAII : public JoinOrDetachPolicy {
public:
	~ThreadRAII(){
//		std::cout << __PRETTY_FUNCTION__ << std::endl;
		if(threadToManage.joinable()) // if not-> ~std::thread() std::terminate() is called
			this->joinOrDetach(threadToManage);
	}
	ThreadRAII(std::thread&& threadToManage) : threadToManage(std::move(threadToManage)){
//		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
	ThreadRAII(ThreadRAII&&) =  default;
	ThreadRAII& operator=(ThreadRAII&&) =  default;

	operator std::thread&(){ return threadToManage; }
	// alternativ:
	std::thread& get(){ return threadToManage; }
private:
	std::thread threadToManage;
};



#endif /* SRC_THREADRAII_H_ */
