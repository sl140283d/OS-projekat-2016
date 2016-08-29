/*
 * idle.h
 *
 *  Created on: Aug 10, 2016
 *      Author: OS1
 */

#include "thread.h"

#ifndef _idle_h_
#define _idle_h_

class idleThread: public Thread{
private:
	static volatile int unblFlag;
	int breakFlag;
public:
	idleThread();
	void run();
	~idleThread();
protected:
	friend class BlockList;
	friend class KernelEv;
};

#endif
