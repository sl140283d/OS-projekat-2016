/*
 * blockLis.cpp
 *
 *  Created on: Aug 17, 2016
 *      Author: OS1
 */

#include "blockLis.h"
#include "schedule.h"
#include "mySemImp.h"

extern int cs_req;

Elem::Elem(ID i, Time t):next(0){
	time = t;
	id = i;
	PCB::getPCBByID(id)->state = blocked;
}

BlockList::BlockList():head(0),tail(0){}

Elem::~Elem(){}

BlockList::~BlockList(){
	Elem *current;
	while ( head ){
		current = head;
		head = head->next;
		delete current;
	}
	head = tail = 0;
}

void BlockList::add(ID id, Time time){
	lock;
	Elem *newElem = new Elem(id, time);
	if ( head == 0 ) head = newElem;
	else tail->next = newElem;
	tail = newElem;
	KernelSem::blCounter++;
	cs_req = 1;
	unlock;
}

void BlockList::remove(){
	lock;
	if ( head == 0 ) return;
	Elem *old = head;
	if ( head == tail ) head = tail = 0;
	else head = head->next;
	PCB::getPCBByID(old->id)->state = ready;
	Scheduler::put(PCB::getPCBByID(old->id));
	PCB::getPCBByID(old->id)->sign = 1;
	idleThread::unblFlag = 1;
	KernelSem::blCounter--;
	delete old;
	unlock;
}

void BlockList::decAndRemove(){
	Elem *prev = 0;
	Elem *curr = head;
	Elem *old;
	while ( curr != 0 ){
		 if ( curr->time != 0 ){
		 curr->time = curr->time - 1;
		 if ( curr->time == 0){
			 PCB::getPCBByID(curr->id)->state = ready;
			 Scheduler::put(PCB::getPCBByID(curr->id));
			 KernelSem::blCounter--;
			 idleThread::unblFlag = 1;
			 old = curr;
			 if ( prev == 0 ) head = head->next;
			 else prev->next = curr->next;
			 if ( curr == tail ) {
				 if ( prev != 0 ) prev->next = 0;
				 tail = prev;
			 }
			 curr = curr->next;
			 delete old;
		 }
		 else {
			 prev = curr;
			 curr = curr->next;
		 }
		 }
		 else{
			 prev = curr;
			 curr = curr->next;
		 }
	}
}

void BlockList::removeAll(){
	lock;
	Elem *old;
	while ( head != 0 ){
		PCB::getPCBByID(head->id)->state = ready;
		Scheduler::put(PCB::getPCBByID(head->id));
		KernelSem::blCounter--;
		idleThread::unblFlag = 1;
		old = head;
		head = head->next;
		delete old;
	}
	unlock;
}

