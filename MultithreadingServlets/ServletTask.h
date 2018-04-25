/*
 * ServiceTask.h
 *
 *  Created on: Oct 10, 2013
 *      Author: pipo
 */

#ifndef SERVICETASK_H_
#define SERVICETASK_H_

#include "RequestResponse.h"

namespace FactorizerServlet {

template<class ServletType>
class ServletTask {
	ValueType number;
	ServletType& servlet;
public:
	ServletTask(ValueType num, ServletType& servlet)
		: number(num), servlet(servlet) {};

	ServletResponse operator()(){
		ServletRequest req(number);
		ServletResponse resp;
		servlet.service(req,resp);

		return resp;
	}
};

} // end namespace FactorizerServlet
#endif /* SERVICETASK_H_ */
