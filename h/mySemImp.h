/*
 * mySemImp.h
 *
 *  Created on: Aug 17, 2016
 *      Author: OS1
 */

#ifndef _mySemImp_h_
#define _mySemImp_h_

#include "semaphor.h"
#include "pcb.h"
#include "blockLis.h"

extern int lockFlag;
extern int cs_req;

class semList;
class BlockList;

class KernelSem{
private:
	static semList *head;
	static semList *tail;
	static int blCounter;
	int value;
	BlockList *blockList;
public:
	KernelSem(int value = 1);
	void signal();
	int wait(Time maxTimeToWait);
	~KernelSem();
protected:
	friend class Semaphore;
	friend class BlockList;
	friend class PCB;
	friend class Thread;
	friend interrupt void timer();
	friend class KernelEv;
};

class semList{
private:
	KernelSem *sem;
	semList *next;
public:
	semList(KernelSem *sem);
	~semList();
protected:
	friend interrupt void timer();
	friend class KernelSem;
};

#endif
