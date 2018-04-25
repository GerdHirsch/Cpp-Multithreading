/*
 * UnsafeCountingFactorizer.h
 *
 *  Created on: Oct 14, 2013
 *      Author: pipo
 */

#ifndef UNSAFECOUNTINGFACTORIZER_H_
#define UNSAFECOUNTINGFACTORIZER_H_

#include <vector>

#include "FactorizerServletBase.h"

namespace FactorizerServlet {

class UnsafeCountingFactorizer : FactorizerServletBase {
	long count = 0;

public:
	void service(ServletRequest& req, ServletResponse& resp) {
		ValueType i = extractFromRequest(req);
		std::vector<ValueType> factors = factor(i);
		++count;
		encodeIntoResponse(resp, i, factors);
	}
};

} // end namespace FactorizerServlet

#endif /* UNSAFECOUNTINGFACTORIZER_H_ */
