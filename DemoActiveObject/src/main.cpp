/*
 * main.cpp
 *
 *  Created on: 11.04.2018
 *      Author: Gerd
 */

#include <iostream>

#include "ActiveObjectBase.h"

using namespace std;

int main(){
	cout << "DemoActiveObject" << endl;

	ActiveObjectBase activeObject;
	cout  << "thread id: " << this_thread::get_id() << endl;
	activeObject.foo();
	this_thread::sleep_for(10ms);
	activeObject.bar();
	this_thread::sleep_for(10ms);
}


