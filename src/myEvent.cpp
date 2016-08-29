/*
 * myEvent.cpp
 *
 *  Created on: Aug 22, 2016
 *      Author: OS1
 */

#include "myEvent.h"
#include "ivtEntry.h"
#include "ivt.h"

class IVTEntry;
extern int cs_req;

KernelEv::KernelEv(int ivtno){
	this->ivtno = ivtno;
	id = PCB::running->getID();
	if ( IVTEntry::ivt.get(ivtno)->event == 0 ) IVTEntry::ivt.get(ivtno)->event = this;
}

void KernelEv::wait(){
	lock;
	if ( PCB::running->getID() == id ) {
		PCB::running->state = blocked;
		KernelSem::blCounter++;
		cs_req = 1;
	}
	unlock;
}

void KernelEv::signal(){
	lock;
	if ( PCB::getPCBByID(id) != 0 ){
		PCB::getPCBByID(id)->state = ready;
		Scheduler::put(PCB::getPCBByID(id));
		idleThread::unblFlag = 1;
		KernelSem::blCounter--;
		cs_req = 1;
	}
	unlock;
}

KernelEv::~KernelEv(){
	if ( IVTEntry::ivt.get(ivtno)->event == this ) IVTEntry::ivt.get(ivtno)->event = 0;
	if ( ( PCB::getPCBByID(id) ) && ( PCB::getPCBByID(id)->state == blocked ) ) signal();
}


