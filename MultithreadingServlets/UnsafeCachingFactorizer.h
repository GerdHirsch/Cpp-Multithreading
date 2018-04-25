/*
 * UnsafeCachingFactorizer.h
 *
 *  Created on: Oct 10, 2013
 *      Author: pipo
 */

#ifndef UNSAFECACHINGFACTORIZER_H_
#define UNSAFECACHINGFACTORIZER_H_

#include <atomic>
#include <vector>

#include "FactorizerServletBase.h"

namespace FactorizerServlet {

class UnsafeCachingFactorizer : FactorizerServletBase {
	std::atomic<FactorizerServlet::ValueType> lastNumber;

	std::mutex factorsMutex;

	std::vector<ValueType> lastFactors;

public:
	void service(ServletRequest& req, ServletResponse& resp) {
        ValueType i = extractFromRequest(req);
        if (i == lastNumber.load())
            encodeIntoResponse(resp, i, lastFactors);
        else
        {
            std::vector<ValueType> factors = factor(i);
            lastNumber.store(i);
            {
        		std::lock_guard<std::mutex> guard(factorsMutex);
            	lastFactors = factors;
            }
            encodeIntoResponse(resp, i, factors);
        }
    }
};

} // end namespace FactorizerServlet

#endif /* UNSAFECACHINGFACTORIZER_H_ */
