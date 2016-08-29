/*
 * thread.cpp
 *
 *  Created on: Jun 22, 2016
 *      Author: OS1
 */

#include "thread.h"
#include "pcb.h"
#include <dos.h>

extern int cs_req;
extern int lockFlag;
extern int deleteFlag;

void Thread::start(){
	myPCB->state = ready;
	Scheduler::put(myPCB);
}

void Thread::waitToComplete(){
	if ( myPCB->state != finished ){
		myPCB->sem->blockList->add(PCB::running->id,0);
	}
}

Thread::~Thread(){
	waitToComplete();
	if ( myPCB != 0 ){
		myPCB->delFlag = 1;
		delete myPCB;
	}
	myPCB = 0;
}

ID Thread::getId(){
	return myPCB->getID();
}

ID Thread::getRunningId(){
	return PCB::running->getID();
}

Thread *Thread::getThreadById(ID id){
	if ( PCB::getPCBByID(id) == 0 ) return 0;
	return PCB::getPCBByID(id)->getThread();
}

Thread::Thread(StackSize stackSize , Time timeSlice ){
	myPCB = new PCB(stackSize, timeSlice, this);
}

void dispatch(){
	#ifndef BCC_BLOCK_IGNORE
	asm cli;
	#endif
	cs_req = 1;
	timer();
	#ifndef BCC_BLOCK_IGNORE
	asm sti;
	#endif
}

ID Thread::fork(){
	lock;
	myFork();
	unlock;
	return PCB::running->forkVal;
}

void interrupt myFork(){
	ID runningID = PCB::running->id;
	Thread *newThread = PCB::running->myThread->clone();
	if ( newThread == 0 ){
		PCB::running->forkVal = -1;
		return;
	}
	ID childID = newThread->getId();
	PCB::getPCBByID(childID)->father = PCB::running;
	PCB::running->numChild++;
	PCB::running->forkVal = childID;
	PCB::getPCBByID(childID)->forkVal = 0;
	unsigned tbp, *pointer;
	int add;
	#ifndef BCC_BLOCK_IGNORE
	asm mov tbp,bp
	pointer = (unsigned *)MK_FP(PCB::running->ss,tbp);
	add = pointer - PCB::running->stack;
	PCB::getPCBByID(childID)->bp = FP_OFF(PCB::getPCBByID(childID)->stack + add);
	#endif
	PCB::getPCBByID(childID)->sp = PCB::getPCBByID(childID)->bp;
	for ( int i = 0 ; i < PCB::running->stackSize ; i++ ) PCB::getPCBByID(childID)->stack[i] = PCB::running->stack[i];
	newThread->start();
}

void Thread::exit(){
	PCB::running->state = finished;
	PCB::running->sem->blockList->removeAll();
	if ( PCB::running->father != 0 ){
		PCB::running->father->fathersSem->signal();
		//delete PCB::running->myThread;
	}
	PCB::running = 0;
	deleteFlag = 1;
	dispatch();
}

void Thread::waitForForkChildren(){
	for ( int i = 0 ; i < PCB::running->numChild ; i++ ) PCB::running->fathersSem->wait(0);
}

Thread* Thread::clone() const{
	return new Thread(PCB::running->stackSize, PCB::running->timeSlice);
}
