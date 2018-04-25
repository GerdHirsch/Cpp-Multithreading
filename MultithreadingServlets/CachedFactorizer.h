/*
 * CachedFactorizer.h
 *
 *  Created on: Oct 10, 2013
 *      Author: pipo
 */

#ifndef CACHEDFACTORIZER_H_
#define CACHEDFACTORIZER_H_

#include <vector>

#include "FactorizerServletBase.h"

namespace FactorizerServlet {

class CachedFactorizer : FactorizerServletBase {
    ValueType lastNumber;
    std::vector<ValueType> lastFactors;
    long hits;
    long cacheHits;
	std::mutex lock;


public:
    long getHits() {
		std::lock_guard<std::mutex> guard(lock);
        return hits;
    }

    double getCacheHitRatio() {
		std::lock_guard<std::mutex> guard(lock);
        return (double) cacheHits / (double) hits;
    }

    void service(ServletRequest& req, ServletResponse& resp) {
        ValueType i = extractFromRequest(req);
        std::vector<ValueType> factors;
        bool cached = false;
        {
			std::lock_guard<std::mutex> guard(lock);
            ++hits;
            if (i == lastNumber) {
                ++cacheHits;
                factors = lastFactors;
                cached = true;
            }
        }
        if (!cached) {
        	factors = factor(i);
            {
				std::lock_guard<std::mutex> guard(lock);
                lastNumber = i;
                lastFactors = factors;
            }
        }
        encodeIntoResponse(resp, i, factors);
    }
};

} // end namespace FactorizerServlet

#endif /* CACHEDFACTORIZER_H_ */
