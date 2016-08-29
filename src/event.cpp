/*
 * event.cpp
 *
 *  Created on: Aug 22, 2016
 *      Author: OS1
 */

#include "event.h"
#include "myEvent.h"
#include "pcb.h"

Event::Event(IVTNo ivtno){
	lock;
	myImpl = new KernelEv(ivtno);
	unlock;
}

void Event::wait(){
	myImpl->wait();
}

void Event::signal(){
	//myImpl->signal();
}

Event::~Event(){
	delete myImpl;
}
