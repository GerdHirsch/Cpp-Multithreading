/*
 * RequestResponse.h
 *
 *  Created on: Oct 10, 2013
 *      Author: pipo
 */

#ifndef REQUESTRESPONSE_H_
#define REQUESTRESPONSE_H_

#include <vector>

namespace FactorizerServlet {

// Typ der in Primzahlen zerlegt werden soll
using ValueType = unsigned long long;

class ServletRequest {
	ValueType number;
public:
	ServletRequest(ValueType num) : number(num) {}

	ValueType getNum(){
		return number;
	}
};

class ServletResponse {
	ValueType number;
	std::vector<ValueType> factors;
public:
	void setNum(ValueType num){
		number = num;
	}
	void setFactors(std::vector<ValueType>& fact){
		factors = fact;
	}
	ValueType getNum(){
		return number;
	}
	std::vector<ValueType>& getFactors(){
		return factors;
	}
};

} // end namespace FactorizerServlet
#endif /* REQUESTRESPONSE_H_ */
