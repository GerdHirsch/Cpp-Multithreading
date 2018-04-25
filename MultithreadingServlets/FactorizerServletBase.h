#ifndef FACTORIZERSERVLETBASE_H_
#define FACTORIZERSERVLETBASE_H_

#include <vector>

#include "RequestResponse.h"

namespace FactorizerServlet {

class FactorizerServletBase {
public:
	ValueType extractFromRequest(ServletRequest& req) {
		return req.getNum();
	}

	void encodeIntoResponse(ServletResponse& resp, ValueType number, std::vector<ValueType>& factors) {
		resp.setNum(number);
		resp.setFactors(factors);
	}

	std::vector<ValueType> factor(ValueType x) {
		std::vector<ValueType> retval;
		ValueType p = 2;
		if(x <= p) {
			retval.push_back(x);
			return retval;
		}
		while((x % p) == 0) {
			retval.push_back(2);
			x /= 2;
		}
		p = 3;
		while(p <= x) {
			if((x % p) == 0) {
				x /= p;
				retval.push_back(p);
			} else {
				p += 2;
			}
		}
		return retval;
	}
};

} // end namespace FactorizerServlet

#endif /* FACTORIZERSERVLETBASE_H_ */
