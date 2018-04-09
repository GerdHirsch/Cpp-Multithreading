/*
 * main.cpp
 *
 *  Created on: 09.04.2018
 *      Author: Gerd
 *  Effective Modern C++, Item 39
 *  see also project StaleData for a loop based bad polling solution
 */

#include <iostream>
using namespace std;

void demoConditionVariable();
void demoPromiseFuture();

int main(){
	cout << "ActivateAnotherTask" << endl;

	cout << endl;
	demoPromiseFuture();
	cout << endl;
	demoConditionVariable();
}


