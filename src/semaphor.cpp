/*
 * semaphor.cpp
 *
 *  Created on: Aug 17, 2016
 *      Author: OS1
 */

#include "semaphor.h"
#include "mySemImp.h"

Semaphore::Semaphore(int init ){
	myImpl = new KernelSem(init);
}
Semaphore::~Semaphore(){
	delete myImpl;
}
int Semaphore::val()const{
	return myImpl->value;
}
int Semaphore::wait(Time maxTimeToWait){
	return myImpl->wait(maxTimeToWait);
}
void Semaphore::signal(){
	myImpl->signal();
}


