/*
 * ActiveObject.h
 *
 *  Created on: 11.04.2018
 *      Author: Gerd
 */

#ifndef ACTIVEOBJECTBASE_H_
#define ACTIVEOBJECTBASE_H_

#include <BlockingQueue/BlockingQueue.h>

#include <memory>
#include <thread>

class ActiveObjectBase{
protected:
	class Command{
	public:
		virtual ~Command(){}
		virtual void doIt(){}// = 0;
	};
	using Message = Command*;
public:
	// suppress copying
	ActiveObjectBase(ActiveObjectBase const&) = delete;
	ActiveObjectBase& operator=(ActiveObjectBase const&) = delete;

	ActiveObjectBase(ActiveObjectBase &&) = default;
	ActiveObjectBase& operator=(ActiveObjectBase &&) = default;

	ActiveObjectBase()
	: thread(&ActiveObjectBase::run, this){}

	~ActiveObjectBase(){
		std::cout << __PRETTY_FUNCTION__ << std::endl;
		std::cout << "thread id: " << std::this_thread::get_id() << std::endl;

		send( done );
		thread.join();
	}
	void foo(){ // UML signal events = asynchronous call
		std::cout << __PRETTY_FUNCTION__ << std::endl;
		std::cout << "thread id: " << std::this_thread::get_id() << std::endl;
		send( fooMessage );
	}
	void bar(){
		std::cout << __PRETTY_FUNCTION__ << std::endl;
		std::cout << "thread id: " << std::this_thread::get_id() << std::endl;
		send( barMessage );
	}
private:
	using MessageQueue = gh::BlockingQueue<Message>;
	using Thread = std::thread;
//	using Thread = std::unique_ptr<std::thread>;

	void send(Command& msg){
		queue.push(&msg);
	}

	void run(){
		Message msg;
		while(queue.pop(msg), msg != &done){
			msg->doIt();
		}
	}
	struct Foo : Command {
		void doIt(){
			std::cout << __PRETTY_FUNCTION__ << std::endl;
			std::cout << "thread id: " << std::this_thread::get_id() << std::endl;
		}
	} fooMessage;
	struct Bar : Command {
		void doIt(){
			std::cout << __PRETTY_FUNCTION__ << std::endl;
			std::cout << "thread id: " << std::this_thread::get_id() << std::endl;
		}
	} barMessage;
	struct Done : Command{
		void doIt(){
			std::cout << __PRETTY_FUNCTION__ << std::endl;
			std::cout << "thread id: " << std::this_thread::get_id() << std::endl;
		}
	} done;
	MessageQueue queue;
	Thread thread;
};



#endif /* ACTIVEOBJECTBASE_H_ */
