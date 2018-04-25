#ifndef COUNTINGFACTORIZER_H_
#define COUNTINGFACTORIZER_H_
#include <atomic>
#include <vector>

#include "FactorizerServletBase.h"

namespace FactorizerServlet {

class CountingFactorizer : FactorizerServletBase {
	std::atomic<long> count;

public:
	void service(ServletRequest& req, ServletResponse& resp) {
		ValueType i = extractFromRequest(req);
		std::vector<ValueType> factors = factor(i);
		++count;
		encodeIntoResponse(resp, i, factors);
	}
};

} // end namespace FactorizerServlet

#endif /* COUNTINGFACTORIZER_H_ */
