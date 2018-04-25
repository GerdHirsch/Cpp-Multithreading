#include <iostream>
#include <future>
#include <chrono>
#include <vector>

#include "ServletTask.h"
#include "CountingFactorizer.h"
#include "UnsafeCountingFactorizer.h"
#include "UnsafeCachingFactorizer.h"
#include "SynchonizedFactorizer.h"
#include "CachedFactorizer.h"

using namespace std;

void printResponse(FactorizerServlet::ServletResponse resp) {
	cout << "Faktoren von " << resp.getNum() << ": ";
	vector<FactorizerServlet::ValueType> factors = resp.getFactors();
	for(auto f : factors){
		cout << f << " ";
	}
	cout << endl;
}

inline
future<FactorizerServlet::ServletResponse> startFactorizerTask(FactorizerServlet::ValueType num){
	using Servlet = FactorizerServlet::UnsafeCountingFactorizer;
//	using Servlet = FactorizerServlet::CountingFactorizer;
//	using Servlet = FactorizerServlet::UnsafeCachingFactorizer;
//	using Servlet = FactorizerServlet::SynchronizedFactorizer;
//	using Servlet = FactorizerServlet::CachedFactorizer;

	using Task = FactorizerServlet::ServletTask<Servlet>;

	static Servlet service;

	return async(launch::async, Task(num, service));
}

int main(){
	cout << "MultithreadingServlets" << endl;

	vector<future<FactorizerServlet::ServletResponse>> tasks;

	tasks.push_back(startFactorizerTask(0));
	tasks.push_back(startFactorizerTask(1));
	tasks.push_back(startFactorizerTask(2));
	tasks.push_back(startFactorizerTask(888121));
	tasks.push_back(startFactorizerTask(53));
	tasks.push_back(startFactorizerTask(10));
	tasks.push_back(startFactorizerTask(11));
	tasks.push_back(startFactorizerTask(16));
	tasks.push_back(startFactorizerTask(196));
	tasks.push_back(startFactorizerTask(27));
	tasks.push_back(startFactorizerTask(23));
	tasks.push_back(startFactorizerTask(3*5*7*11*13*17*19*23));


	for(auto& f : tasks)
		printResponse(f.get());
}
