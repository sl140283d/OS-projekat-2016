/*
 * pcb.h
 *
 *  Created on: Jun 22, 2016
 *      Author: OS1
 */

#ifndef _pcb_h_
#define _pcb_h_

#include "thread.h"
#include "cswitch.h"
#include "schedule.h"
#include "idle.h"
#include "pcbStack.h"
#include "mySemImp.h"
#include "blockLis.h"

typedef unsigned REG;
const StackSize maxStackSize = 65535;
enum States{ created , ready , blocked , finished };
class idleThread;
class PCBStack;
class BlockList;

class PCB{
private:
	static ID oldID;
	static PCBStack pcbStack;
	static idleThread IDLET;
	static PCB *IDLE;
	KernelSem *sem;
	Thread *myThread;
	ID id;
	Time timeSlice;
	REG *stack;
	REG sp;
	REG ss;
	REG bp;
	int delFlag;
	StackSize stackSize;
	States state;
	int sign;
	REG numChild;
	KernelSem *fathersSem;
	PCB* father;
	ID forkVal;
public:
	PCB (StackSize stackSize, Time timeSlice, Thread *thread);
	ID getID();
	static PCB* getPCBByID(ID id);
	Thread* getThread();
	void createProcess();
	static PCB* running;
	static void wrapper();
	~PCB();
protected:
	void setSpBp(REG sp, REG ss, REG bp);
	friend class Thread;
	friend void restore();
	friend interrupt void timer();
	friend interrupt void myFork();
	friend class Elem;
	friend class BlockList;
	friend class KernelSem;
	friend class KernelEv;
	friend class idleThread;
};
#endif
