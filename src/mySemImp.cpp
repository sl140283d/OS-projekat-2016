/*
 * mySemImp.cpp
 *
 *  Created on: Aug 17, 2016
 *      Author: OS1
 */

#include "mySemImp.h"

semList* KernelSem::head = 0;
semList* KernelSem::tail = 0;
int KernelSem::blCounter = 0;

KernelSem::KernelSem(int val){
	value = val;
	lock;  // <--------------
	blockList = new BlockList();
	semList *newSem = new semList(this);
	if ( KernelSem::head == 0 ) KernelSem::head = newSem;
	else KernelSem::tail->next = newSem;
	KernelSem::tail = newSem;
	unlock;  //
}

void KernelSem::signal(){
	int val;
	lock;
	val = ++value;
	unlock;
	if ( val <= 0 ){
		blockList->remove();
	}
}

int KernelSem::wait(Time maxTime){
	int val;
	lock;
	val = --value;
	unlock;
	int retVal;
	PCB::running->sign = 0;
	if ( val < 0 ){
		blockList->add(PCB::running->getID(), maxTime);
	}
	return PCB::running->sign;
}

KernelSem::~KernelSem(){
	lock;
	semList *prev = 0;
	semList *curr = KernelSem::head;
	while ( curr->sem != this ) {
		prev = curr;
		curr = curr->next;
	}
	if ( prev == 0 ){
		KernelSem::head = KernelSem::head->next;
		if ( curr == KernelSem::tail ) KernelSem::tail = 0;
		delete curr;
	}
	else{
		prev->next = curr->next;
		delete curr;
	}
	delete blockList;
	unlock;
}


semList::semList(KernelSem *se):sem(se), next(0){}

semList::~semList(){}
