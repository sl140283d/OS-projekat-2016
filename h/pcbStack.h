/*
 * pcbStack.h
 *
 *  Created on: Aug 16, 2016
 *      Author: OS1
 */

#ifndef _pcbStack_h_
#define _pcbStack_h_

#include "pcb.h"

class PCBStack{
private:
	PCB** pcbStack;
	StackSize size;
	void put(PCB *pcb,unsigned long id);
	PCB* get(unsigned long id);
public:
	PCBStack();
	~PCBStack();
protected:
	friend class PCB;
};



#endif
