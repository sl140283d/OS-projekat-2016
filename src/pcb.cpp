/*
 * pcb.cpp
 *
 *  Created on: Jun 22, 2016
 *      Author: OS1
 */

#include "pcb.h"
#include <dos.h>

extern int deleteFlag;
ID PCB::oldID = 0;

PCBStack PCB::pcbStack;
idleThread PCB::IDLET;
PCB* PCB::IDLE = PCB::IDLET.myPCB;
PCB* PCB::running = 0;

PCB::PCB (StackSize stackSize, Time timeSlice, Thread* thread){
		sem = new KernelSem(0);
		id = oldID++;
		int sign = 0;
		numChild = 0;
		this->timeSlice = timeSlice;
		this->myThread = thread;
		if ( stackSize > maxStackSize ) this->stackSize = maxStackSize;
		else this->stackSize = stackSize;
		this->stack = new REG[this->stackSize];
		pcbStack.put(this,id);
		delFlag = 0;
		father = 0;
		fathersSem = new KernelSem(0);
		createProcess();
}

ID PCB::getID() { return id; }

void PCB::setSpBp(REG sp, REG ss, REG bp){
	this->sp = sp;
	this->ss = ss;
	this->bp = bp;
}
PCB* PCB::getPCBByID(ID id) {
	if ( id >= oldID ) return 0;
	return pcbStack.get(id);
}

Thread* PCB::getThread() { return myThread; }

PCB::~PCB(){
	delete sem;
	delete fathersSem;
	delete stack;
	father = 0;
	if ( delFlag == 0 ) delete myThread;
	myThread = 0;
	pcbStack.put(0,id);
}

void PCB::wrapper(){
	PCB::running->myThread->run();
	PCB::running->state = finished;
	PCB::running->sem->blockList->removeAll();
	if ( PCB::running->father != 0 ){
		PCB::running->father->fathersSem->signal();
		//delete PCB::running->myThread;
		//PCB::running = 0;
		//deleteFlag = 1;
	}
	dispatch();
}

void PCB::createProcess(){
	stack[stackSize - 1] = 0x200;
	#ifndef BCC_BLOCK_IGNORE
		stack[stackSize - 2] = FP_SEG(PCB::wrapper);
		stack[stackSize - 3] = FP_OFF(PCB::wrapper);
		sp = FP_OFF(stack + stackSize - 12);
		ss = FP_SEG(stack + stackSize - 12);
		bp = FP_OFF(stack + stackSize - 12);
	#endif
	this->state = created;
}

