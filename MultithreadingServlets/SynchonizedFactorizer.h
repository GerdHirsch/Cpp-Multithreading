/*
 * SynchonizedFactorizer.h
 *
 *  Created on: Oct 10, 2013
 *      Author: pipo
 */

#ifndef SYNCHONIZEDFACTORIZER_H_
#define SYNCHONIZEDFACTORIZER_H_

#include <mutex>
#include <vector>

#include "FactorizerServletBase.h"


namespace FactorizerServlet {

class SynchronizedFactorizer : FactorizerServletBase {
    ValueType lastNumber;
    std::vector<ValueType> lastFactors;
    std::mutex factorsMutex;

public:
    void service(ServletRequest& req, ServletResponse& resp) {
		std::lock_guard<std::mutex> guard(factorsMutex);
    	std::cout << "SynchronizedFactorizer::service(ServletRequest req, ServletResponse resp)" << std::endl;

    	ValueType i = extractFromRequest(req);
        if (i == lastNumber) {
            encodeIntoResponse(resp, lastNumber, lastFactors);
        } else {
            std::vector<ValueType> factors = factor(i);
            lastNumber = i;
            lastFactors = factors;
            encodeIntoResponse(resp, lastNumber, lastFactors);
        }
    }
};

} // end namespace FactorizerServlet

#endif /* SYNCHONIZEDFACTORIZER_H_ */
