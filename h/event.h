/*
 * event.h
 *
 *  Created on: Aug 22, 2016
 *      Author: OS1
 */

#ifndef _event_h_
#define _event_h_

#include "ivt.h"
#include "ivtEntry.h"
#include "myEvent.h"

typedef unsigned char IVTNo;
class KernelEv;

class Event {
public:
	Event (IVTNo ivtNo);
	~Event ();
	void wait ();
protected:
	friend class KernelEv;
	void signal(); // can call KernelEv
private:
	KernelEv* myImpl;
};

#endif
