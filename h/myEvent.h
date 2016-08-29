/*
 * myEvent.h
 *
 *  Created on: Aug 22, 2016
 *      Author: OS1
 */

#ifndef _myEvent_h_
#define _myEvent_h_

#include "event.h"
#include "pcb.h"

class KernelEv{
private:
	int ivtno;
	ID id;
public:
	KernelEv(int ivtno);
	~KernelEv();
	void wait();
protected:
	void signal();
	friend class Event;
	friend class IVTEntry;
};

#endif
